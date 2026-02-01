#pragma once

#include <memory>
#include <thread>
#include <mutex>
#include <deque>
#include <optional>
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <cstdint>

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

namespace network
{
template <typename T> class tsqueue
{
  public:
    tsqueue() = default;
    tsqueue(const tsqueue<T>&) = delete;
    virtual ~tsqueue()
    {
        clear();
    }

    const T& front()
    {
        std::scoped_lock lock(muxQueue);
        return deqQueue.front();
    }
    const T& back()
    {
        std::scoped_lock lock(muxQueue);
        return deqQueue.back();
    }
    void push_back(const T& item)
    {
        std::scoped_lock lock(muxQueue);
        deqQueue.emplace_back(std::move(item));

        std::unique_lock<std::mutex> ul(muxBlocking);
        cvBlocking.notify_one();
    }
    void push_front(const T& item)
    {
        std::scoped_lock lock(muxQueue);
        deqQueue.emplace_front(std::move(item));

        std::unique_lock<std::mutex> ul(muxBlocking);
        cvBlocking.notify_one();
    }
    bool empty()
    {
        std::scoped_lock lock(muxQueue);
        return deqQueue.empty();
    }
    size_t count()
    {
        std::scoped_lock lock(muxQueue);
        return deqQueue.size();
    }
    void clear()
    {
        std::scoped_lock lock(muxQueue);
        deqQueue.clear();
    }
    T pop_front()
    {
        std::scoped_lock lock(muxQueue);
        auto t = std::move(deqQueue.front());
        deqQueue.pop_front();

        return t;
    }
    T pop_back()
    {
        std::scoped_lock lock(muxQueue);
        auto t = std::move(deqQueue.front());
        deqQueue.pop_back();

        return t;
    }
    void wait()
    {
        while (empty())
        {
            std::unique_lock<std::mutex> ul(muxBlocking);
            cvBlocking.wait(ul);
        }
    }

  protected:
    std::mutex muxQueue;
    std::deque<T> deqQueue;
    std::condition_variable cvBlocking;
    std::mutex muxBlocking;
};

template <typename T> struct messageHeader
{
    T id{};
    uint32_t size = 0;
};

template <typename T> struct message
{
    messageHeader<T> header{};
    std::vector<uint8_t> body;

    size_t size() const
    {
        return sizeof(messageHeader<T>) + body.size();
    }

    friend std::ostream& operator<<(std::ostream& os, const message<T>& msg)
    {
        os << "ID: " << int(msg.header.id) << " Size: " << msg.header.size;
        return os;
    }

    template <typename dataType> friend message<T>& operator<<(message<T>& msg, dataType& data)
    {
        static_assert(std::is_standard_layout<dataType>::value, "Data is too complex to be pushed into the vector.");

        size_t start = msg.body.size();

        msg.body.resize(msg.body.size() + sizeof(dataType));
        std::memcpy(msg.body.data() + start, &data, sizeof(dataType));
        // appendind at the end of the body vector the additional data with it's size

        msg.header.size = msg.size();

        return msg;
    }

    template <typename dataType> friend message<T>& operator>>(message<T>& msg, dataType& data)
    {
        static_assert(std::is_standard_layout<dataType>::value, "Data is too complex to be pushed into the vector.");

        size_t end = msg.body.size() - sizeof(dataType);

        std::memcpy(&data, msg.body.data() + end, sizeof(dataType));
        msg.body.resize(end);

        msg.header.size = msg.size();

        return msg;
    }
};

template <typename T> class serverInterface
{
  public:
    serverInterface(uint16_t port) : m_asioAcceptor(m_asioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
    {
    }
    virtual ~serverInterface()
    {
        Stop();
    }
    bool Start()
    {
        try
        {
            WaitForClientConnection();

            m_threadContext = std::thread([this]() { m_asioContext.run(); });
        }
        catch (std::exception& e)
        {
            std::cerr << "[SERVER] exception: " << e.what() << "\n";
            return false;
        }

        std::cout << "[SERVER] started!\n";
        return true;
    }
    void Stop()
    {
        m_asioContext.stop();

        if (m_threadContext.joinable())
            m_threadContext.join();

        std::cout << "[SERVER] stopped!\n";
    }
    void WaitForClientConnection()
    {
        m_asioAcceptor.async_accept(
            [this](std::error_code ec, asio::ip::tcp::socket socket)
            {
                if (!ec)
                {
                    std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << "\n";
                    std::shared_ptr<connection<T>> newconn = std::make_shared<connection<T>>(
                        connection<T>::owner::server, m_asioContext, std::move(socket), m_qMessagesIn);

                    if (OnClientConnect(newconn))
                    {
                        m_deqConnections.push_back(std::move(newconn));
                        m_deqConnections.back()->ConnectToClient(this, nIDCounter++);

                        std::cout << "[" << m_deqConnections.back()->GetID() << "] Connection Approved\n";
                    }
                    else
                    {
                        std::cout << "[SERVER] Connection Denied\n";
                    }
                }
                else
                {
                    std::cout << "[SERVER] New Connection Error: " << ec.message() << "\n";
                }
                WaitForClientConnection();
            });
    }
    void MessageClient(std::shared_ptr<connection<T>> client, const message<T>& msg)
    {
        if (client && client->IsConnected())
        {
            client->Send(msg);
        }
        else
        {
            OnClientDisconnect(client);
            client.reset();
            m_deqConnections.erase(std::remove(m_deqConnections.begin(), m_deqConnections.end(), client),
                                   m_deqConnections.end());
        }
    }
    void MessageAllClient(const message<T>& msg, std::shared_ptr<connection<T>> pIngoreClient)
    {
        bool bInvalidClientExists = false;

        for (auto& client : m_deqConnections)
        {
            if (client && client->IsConnected())
            {
                if (client != pIngoreClient)
                    client->Send(msg);
            }
            else
            {
                OnClientDisconnect(client);
                client.reset();
                bInvalidClientExists = true;
            }
        }

        if (bInvalidClientExists)
        {
            m_deqConnections.erase(std::remove(m_deqConnections.begin(), m_deqConnections.end(), nullptr),
                                   m_deqConnections.end());
        }
    }
    void Update(size_t nMaxMessages = -1, bool bWait = false)
    {
        if (bWait)
            m_qMessagesIn.wait();

        size_t nMessageCount = 0;
        while (nMessageCount < nMaxMessages && !m_qMessagesIn.empty())
        {
            auto msg = m_qMessagesIn.pop_front();
            OnMessage(msg.remote, msg.msg);
            nMessageCount++;
        }
    }

  protected:
    // when a client connects
    virtual bool OnClientConnect(std::shared_ptr<connection<T>> client)
    {

        return false;
    }

    // when a client disconnects
    virtual void OnClientDisconnect(std::shared_ptr<connection<T>> client)
    {
    }
    // when a message arrives from a client
    virtual void OnMessage(std::shared_ptr<connection<T>> client, message<T>& msg)
    {
    }

  public:
    virtual void OnClientValidated(std::shared_ptr<connection<T>> client)
    {
    }

  protected:
    tsqueue<owned_message<T>> m_qMessagesIn;

    // container of active connections
    std::deque<std::shared_ptr<connection<T>>> m_deqConnections;

    asio::io_context m_asioContext;
    std::thread m_threadContext;

    asio::ip::tcp::acceptor m_asioAcceptor;
    uint32_t nIDCounter = 10000;
};

template <typename T> class connection : public std::enable_shared_from_this<connection<T>>
{
  public:
    enum class owner
    {
        server,
        client
    };

    connection(owner parent, asio::io_context& asioContext, asio::ip::tcp::socket socket,
               tsqueue<owned_message<T>>& qIn)
        : m_asioContext(asioContext), m_socket(std::move(socket)), m_qMessagesIn(qIn)
    {
        m_nOwnerType = parent;

        if (m_nOwnerType == owner::server)
        {
            m_nHandshakeOut = uint64_t(std::chrono::system_clock::now().time_since_epoch().count());
            m_nHandshakeCheck = scramble(m_nHandshakeOut);
        }
        else
        {
            m_nHandshakeIn = 0;
            m_nHandshakeOut = 0;
        }
    };
    virtual ~connection() {};

    void ConnectToClient(network::serverInterface<T>* server, uint32_t uid = 0)
    {
        if (m_nOwnerType == owner::server)
        {
            if (m_socket.is_open())
            {
                id = uid;
                WriteValidation();
                ReadValidation(server);
            }
        }
    }
    void ConnectToServer(asio::ip::tcp::resolver::results_type& endpoints)
    {
        if (m_nOwnerType == owner::client)
        {
            asio::async_connect(m_socket, endpoints,
                [this](std::error_code ec, asio::ip::tcp::endpoint endpoint)
                {
                    if (!ec)
                    {
                        ReadValidation();
                    }
                });
        }
    }
    void Disconnect()
    {
        if (IsConnected())
            asio::post(m_asioContext, [this]() { m_socket.close(); });
    }
    bool IsConnected() const
    {
        return m_socket.is_open();
    }

    void Send(const message<T>& msg)
    {
        asio::post(m_asioContext,
            [this, msg]()
            {
                bool bWritingMessage = !m_qMessagesOut.empty();
                m_qMessagesOut.push_back(msg);
                if (!bWritingMessage)
                {
                    WriteHeader();
                }
            });
    }

    uint32_t GetID() const
    {
        return id;
    }

  private:
    void ReadHeader()
    {
        asio::async_read(m_socket, asio::buffer(&m_msgTemporaryIn.header, sizeof(messageHeader<T>)),
            [this](std::error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    if (m_msgTemporaryIn.header.size > 0)
                    {
                        m_msgTemporaryIn.body.resize(m_msgTemporaryIn.header.size);
                        ReadBody();
                    }
                    else
                    {
                        AddToIncomingMessageQueue();
                    }
                }
                else
                {
                    std::cout << "[" << id << "] Read Header Fail.\n";
                    m_socket.close();
                }
            });
    }
    void ReadBody()
    {
        asio::async_read(m_socket, asio::buffer(m_msgTemporaryIn.body.data(), m_msgTemporaryIn.header.size),
            [this](std::error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    AddToIncomingMessageQueue();
                }
                else
                {
                    std::cout << "[" << id << "] Read Body Fail.\n";
                    m_socket.close();
                }
            });
    }
    void WriteHeader()
    {
        asio::async_write(m_socket, asio::buffer(&m_qMessagesOut.front().header, sizeof(messageHeader<T>)),
            [this](std::error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    if (m_qMessagesOut.front().body.size() > 0)
                    {
                        WriteBody();
                    }
                    else
                    {
                        m_qMessagesOut.pop_front();
                        if (!m_qMessagesOut.empty())
                        {
                            WriteHeader();
                        }
                    }
                }
                else
                {
                    std::cout << "[" << id << "] Write Header Fail.\n";
                    m_socket.close();
                }
            });
    }
    void WriteBody()
    {
        asio::async_write(m_socket, asio::buffer(m_qMessagesOut.front().body.data(), m_qMessagesOut.front().body.size()),
            [this](std::error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    m_qMessagesOut.pop_front();
                    if (!m_qMessagesOut.empty())
                    {
                        WriteHeader();
                    }
                }
                else
                {
                    std::cout << "[" << id << "] Write Body Fail.\n";
                    m_socket.close();
                }
            });
    }

    void AddToIncomingMessageQueue()
    {
        if (m_nOwnerType == owner::server)
            m_qMessagesIn.push_back({this->shared_from_this(), m_msgTemporaryIn});

        else
            m_qMessagesIn.push_back({nullptr, m_msgTemporaryIn});

        ReadHeader();
    }

    uint64_t scramble(uint64_t nInput)
    {
        uint64_t out = nInput ^ 0xDEADCAFEC0DEBEEF;
        out = (out & 0xF0F0F0F0F0F0F0F0) >> 4 | (out & 0x0F0F0F0F0F0F0F0F) << 4;
        return out ^ 0xBEEFFACE030708FF;
    }

    void WriteValidation()
    {
        asio::async_write(m_socket, asio::buffer(&m_nHandshakeOut, sizeof(uint64_t)),
            [this](std::error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    if (m_nOwnerType == owner::client)
                        ReadHeader();
                }
                else
                {
                    std::cout << "Client disconnected (Fail validation)\n";
                    m_socket.close();
                }
            });
    }
    void ReadValidation(network::serverInterface<T>* server = nullptr)
    {
        asio::async_read(m_socket, asio::buffer(&m_nHandshakeIn, sizeof(uint64_t)),
            [this, server](std::error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    if (m_nOwnerType == owner::server)
                    {
                        if (m_nHandshakeIn == m_nHandshakeCheck)
                        {
                            std::cout << "Client validated\n";
                            server->OnClientValidated(this->shared_from_this());

                            ReadHeader();
                        }
                        else
                        {
                            std::cout << "Client disconnected (Fail validation)\n";
                            m_socket.close();
                        }
                    }
                    else
                    {
                        m_nHandshakeOut = scramble(m_nHandshakeIn);
                        WriteValidation();
                    }
                }
                else
                {
                    std::cout << "Client disconnected (Bigger Fail validation)\n";
                    m_socket.close();
                }
            });
    }

  protected:
    asio::ip::tcp::socket m_socket;
    asio::io_context& m_asioContext;

    tsqueue<message<T>> m_qMessagesOut;
    tsqueue<owned_message<T>>& m_qMessagesIn;
    message<T> m_msgTemporaryIn;

    owner m_nOwnerType = owner::server;
    uint32_t id = 0;

    uint64_t m_nHandshakeOut = 0;
    uint64_t m_nHandshakeIn = 0;
    uint64_t m_nHandshakeCheck = 0;
};

template <typename T> struct owned_message
{
    std::shared_ptr<connection<T>> remote = nullptr;
    message<T> msg;

    friend std::ostream& operator<<(std::ostream& os, const owned_message<T>& msg)
    {
        os << msg.msg;
        return os;
    }
};

template <typename T> class clientInterface
{
  public:
    clientInterface()
    {
    }

    virtual ~clientInterface()
    {
        Disconnect();
    }
    void Send(const message<T>& msg)
    {
        if (IsConnected())
            m_connection->Send(msg);
    }
    bool Connect(const std::string& host, const uint16_t port)
    {
        try
        {
            asio::ip::tcp::resolver resolver(m_context);
            asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

            m_connection = std::make_unique<connection<T>>(connection<T>::owner::client, m_context,
                                                           asio::ip::tcp::socket(m_context), m_qMessagesIn);

            m_connection->ConnectToServer(endpoints);

            thrContext = std::thread([this]() { m_context.run(); });
        }
        catch (std::exception& e)
        {
            std::cerr << "Client exception: " << e.what() << "\n";
            return false;
        }

        return true;
    }
    void Disconnect()
    {
        if (IsConnected())
            m_connection->Disconnect();

        m_context.stop();

        if (thrContext.joinable())
            thrContext.join();

        m_connection.release();
    }
    bool IsConnected()
    {
        if (m_connection)
            return m_connection->IsConnected();

        else
            return false;
    }
    tsqueue<owned_message<T>>& Incoming()
    {
        return m_qMessagesIn;
    }

  protected:
    asio::io_context m_context;
    std::thread thrContext;
    std::unique_ptr<connection<T>> m_connection;

  private:
    tsqueue<owned_message<T>> m_qMessagesIn;
};
} // namespace network
