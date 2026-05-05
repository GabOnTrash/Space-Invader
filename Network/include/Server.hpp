#pragma once

#include "Common.hpp"
#include "TsQueue.hpp"
#include "Message.hpp"
#include "Connection.hpp"

// to generate a local certificate valid for 1 year: openssl req -newkey rsa:2048 -nodes -keyout server.key -x509 -days 365 -out server.crt
namespace network
{
	template <typename T>
	class serverInterface
	{
	public:
		serverInterface(uint16_t port) 
			: m_asioAcceptor(m_asioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
			m_sslContext(asio::ssl::context::tls_server)
		{
			this->port = port;
			m_asioAcceptor.set_option(asio::socket_base::reuse_address(true));

			try
			{
				// rejects sslv3 and v2 connections
				m_sslContext.set_options(asio::ssl::context::default_workarounds
					| asio::ssl::context::no_sslv2
					| asio::ssl::context::no_sslv3);

				m_sslContext.use_certificate_chain_file("auth/space_invader_server.crt");
				m_sslContext.use_private_key_file("auth/space_invader_server.key", asio::ssl::context::pem);
			}
			catch (std::exception& e)
			{
				LOG_ERROR_EVERYWHERE(std::string("SSL Setup Error: ") + e.what());
			}
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
				LOG_ERROR_EVERYWHERE(e.what());
				return false;
			}

			LOG_INFO_EVERYWHERE("-- Server started on port " + std::to_string(port) + "! --");
            return true;
        }
		void Stop()
		{
            m_asioContext.stop();

			if (m_threadContext.joinable())
                m_threadContext.join();

			LOG_INFO_CONSOLE("-- Server stopped! --");
		}
		void WaitForClientConnection()
		{
			m_asioAcceptor.async_accept(
				[this](std::error_code ec, asio::ip::tcp::socket socket)
				{
					if (!ec)
					{
						LOG_INFO_EVERYWHERE("New Connection: " + socket.remote_endpoint().address().to_string());
						std::shared_ptr<connection<T>> newconn =
							std::make_shared<connection<T>>(connection<T>::owner::server,
								m_asioContext, m_sslContext, std::move(socket), m_qMessagesIn);

						if (OnClientConnect(newconn))
						{
							if (m_deqConnections.empty()) nIDCounter = 0;

                            m_deqConnections.push_back(std::move(newconn));
                            m_deqConnections.back()->ConnectToClient(this, nIDCounter++);

							LOG_INFO_CONSOLE("[Player #" + std::to_string(m_deqConnections.back()->GetID()) + "] Connection approved");
						}
						else
							LOG_WARN_FILE("Connection denied from server");
					}
					else
						LOG_ERROR_EVERYWHERE("New Connection Error: " + ec.message());

					WaitForClientConnection();
                });
		}
		void MessageClient(std::shared_ptr<connection<T>> client, const message<T>& msg)
		{
			if (client && client->IsConnected())
				client->Send(msg);
		}
		void MessageAllClient(const message<T>& msg, std::shared_ptr<connection<T>> pIgnoreClient)
		{
            for (auto& client : m_deqConnections)
            {
	            if (client && client->IsConnected())
	            {
	            	if (client != pIgnoreClient)
	            		client->Send(msg);
	            }
            }
		}
		void CleanConnections()
		{
			for (auto& client : m_deqConnections)
			{
				if (client && !client->IsConnected())
					OnClientDisconnect(client);
			}

			m_deqConnections.erase(
			std::remove_if(m_deqConnections.begin(), m_deqConnections.end(),
				[this](const std::shared_ptr<connection<T>>& c){ return !c->IsConnected(); }),
			m_deqConnections.end());
		}
		void Update(size_t nMaxMessages = -1, bool bWait = false)
		{
            if (bWait)
            	m_qMessagesIn.wait();

			CleanConnections();
            size_t nMessageCount = 0;

			while (nMessageCount < nMaxMessages && !m_qMessagesIn.empty())
			{
				auto msg = m_qMessagesIn.pop_front();

				if (!msg.remote || !msg.remote->IsConnected())
					continue;

				OnMessage(msg.remote, msg.msg);
				nMessageCount++;
            }
		}

    protected:
        // called when a client connects, disconnects , gets validated and on message arrival
		virtual bool OnClientConnect(std::shared_ptr<connection<T>> client) { return false; }
		virtual void OnClientDisconnect(std::shared_ptr<connection<T>> client) {}
		virtual void OnMessage(std::shared_ptr<connection<T>> client, message<T>& msg) {}

	public:
		virtual void OnClientValidated(std::shared_ptr<connection<T>> client) {}

	protected:
		tsqueue<owned_message<T>> m_qMessagesIn;

		// container of active connections
		std::deque<std::shared_ptr<connection<T>>> m_deqConnections;

		asio::io_context m_asioContext;
		asio::ssl::context m_sslContext;
		std::thread m_threadContext;

		asio::ip::tcp::acceptor m_asioAcceptor;
        uint32_t nIDCounter = 0;
		uint16_t port = 0;
	};
}