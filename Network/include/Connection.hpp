#pragma once

#include "TsQueue.hpp"
#include "Server.hpp"
#include "Message.hpp"

namespace network
{
    template <typename T>
    class serverInterface;

    template <typename T>
    class connection : public std::enable_shared_from_this<connection<T>>
    {
    public:
        enum class owner
        {
            server,
            client
        };

        connection(owner parent, asio::io_context& asioContext, asio::ssl::context& sslContext,
                    asio::ip::tcp::socket socket, tsqueue<owned_message<T>>& qIn)
            : m_asioContext(asioContext), m_socket(std::move(socket), sslContext), m_qMessagesIn(qIn)
        {
            m_nOwnerType = parent;
        }
        virtual ~connection() {}

        void ConnectToClient(network::serverInterface<T>* server, uint32_t uid = 0)
        {
            if (m_nOwnerType == owner::server)
            {
                if (m_socket.lowest_layer().is_open())
                {
                    id = uid;

                    m_socket.async_handshake(asio::ssl::stream_base::server,
                        [this, server](std::error_code ec)
                        {
                            if (!ec)
                            {
                                m_bHandshakeComplete = true;
                                LOG_INFO_EVERYWHERE("Client #" + std::to_string(id) + " SSL handshake validated");
                                server->OnClientValidated(this->shared_from_this());
                                ReadHeader();
                            }
                            else
                            {
                                LOG_ERROR_EVERYWHERE("Client #" + std::to_string(id) + " disconnected: SSL handshake failed (" + ec.message() + ")");
                                asio::error_code ec;
                                m_socket.lowest_layer().close(ec);
                            }
                        });
                }
            }
        }
        void ConnectToServer(asio::ip::tcp::resolver::results_type& endpoints)
        {
            if (m_nOwnerType == owner::client)
            {
                asio::async_connect(m_socket.lowest_layer(), endpoints,
                    [this](std::error_code ec, asio::ip::tcp::endpoint endpoint)
                    {
                        if (!ec)
                        {
                            m_socket.async_handshake(asio::ssl::stream_base::client,
                                [this](std::error_code ec)
                                {
                                    if (!ec)
                                    {
                                        m_bHandshakeComplete = true;
                                        ReadHeader();
                                    }
                                    else
                                    {
                                        LOG_ERROR_EVERYWHERE("SSL handshake failed: " + ec.message());
                                        asio::error_code close_ec;
                                        m_socket.lowest_layer().close(close_ec);
                                    }
                                });
                        }
                    });
            }
        }
        void Disconnect()
        {
            if (IsConnected())
                asio::post(m_asioContext, [this]() { asio::error_code ec; m_socket.lowest_layer().close(ec); });
        }
        bool IsConnected() const
        {
            return m_socket.lowest_layer().is_open();
        }
        bool HasHandshakeHappened() const
        {
            return m_bHandshakeComplete;
        }

        void Send(const message<T>& msg)
        {
            asio::post(m_asioContext,
                [this, msg]()
                {
                    bool bWritingMessage = !m_qMessagesOut.empty();
                    m_qMessagesOut.push_back(msg);

                    if (!bWritingMessage)
                        WriteHeader();
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
                            AddToIncomingMessageQueue();
                    }
                    else
                    {
                        LOG_WARN_EVERYWHERE("[Connection #" + std::to_string(id) + "] Read Header Fail.");
                        asio::error_code ec;
                        m_socket.lowest_layer().close(ec);

                        if (m_nOwnerType == owner::server)
                        {
                            message<T> msgDummy;
                            m_qMessagesIn.push_back({ this->shared_from_this(), msgDummy });
                        }
                    }
                });
        }
        void ReadBody()
        {
            asio::async_read(m_socket, asio::buffer(m_msgTemporaryIn.body.data(), m_msgTemporaryIn.header.size),
                [this](std::error_code ec, std::size_t length)
                {
                    if (!ec)
                        AddToIncomingMessageQueue();
                    else
                    {
                        LOG_WARN_EVERYWHERE("[Connection #" + std::to_string(id) + "] Read Body Fail.");
                        asio::error_code ec;
                        m_socket.lowest_layer().close();
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
                            WriteBody();

                        else
                        {
                            m_qMessagesOut.pop_front();
                            if (!m_qMessagesOut.empty())
                                WriteHeader();
                        }
                    }
                    else
                    {
                        LOG_WARN_EVERYWHERE("[Connection #" + std::to_string(id) + "] Write Header Fail.");
                        asio::error_code ec;
                        m_socket.lowest_layer().close(ec);
                    }
                });
        }
        void WriteBody()
        {
            asio::async_write(m_socket, asio::buffer(m_qMessagesOut.front().body.data(),
                m_qMessagesOut.front().body.size()),
                [this](std::error_code ec, std::size_t length)
                {
                    if (!ec)
                    {
                        m_qMessagesOut.pop_front();
                        if (!m_qMessagesOut.empty())
                            WriteHeader();
                    }
                    else
                    {
                        LOG_WARN_EVERYWHERE("[Connection #" + std::to_string(id) + "] Write Body Fail.");
                        asio::error_code ec;
                        m_socket.lowest_layer().close(ec);
                    }
                });
        }

        void AddToIncomingMessageQueue()
        {
            if (m_nOwnerType == owner::server)
                m_qMessagesIn.push_back({ this->shared_from_this(), m_msgTemporaryIn });

            else
                m_qMessagesIn.push_back({ nullptr, m_msgTemporaryIn });

            ReadHeader();
        }

    protected:
        asio::ssl::stream<asio::ip::tcp::socket> m_socket;
        asio::io_context& m_asioContext;

        tsqueue<message<T>> m_qMessagesOut;
        tsqueue<owned_message<T>>& m_qMessagesIn;

        bool m_bHandshakeComplete = false;	// to secure if the connection has really happened
        message<T> m_msgTemporaryIn;
        
        owner m_nOwnerType = owner::server;
        uint32_t id = 0;
    };
}