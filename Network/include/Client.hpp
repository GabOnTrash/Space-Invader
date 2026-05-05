#pragma once

#include "Common.hpp"
#include "TsQueue.hpp"
#include "Message.hpp"
#include "Connection.hpp"

namespace network
{
	template <typename T>
	class clientInterface
	{
	public:
		clientInterface()
			: m_sslContext(asio::ssl::context::tls_client)
		{
			// local certificate
			m_sslContext.set_verify_mode(asio::ssl::context::verify_peer);

			try
			{
				m_sslContext.load_verify_file("auth/space_invader_server.crt");
			}
			catch (std::exception& e)
			{
				LOG_ERROR_EVERYWHERE(std::string("Error loading the certificate: ") + e.what());
			}
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

				m_connection = std::make_unique<connection<T>>(
					connection<T>::owner::client, 
					m_context,
					m_sslContext,
                    asio::ip::tcp::socket(m_context), 
					m_qMessagesIn);

				m_connection->ConnectToServer(endpoints);
				thrContext = std::thread([this]() { m_context.run(); });
			}
			catch (std::exception& e)
			{
				LOG_ERROR_EVERYWHERE(e.what());
				std::cerr << e.what() << std::endl;
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

			m_connection.reset();
		}
		bool IsConnected()
		{
			if (m_connection)
				return m_connection->IsConnected();

			return false;
		}
		bool HasHandshakeHappened()
		{
			if (m_connection)
				return m_connection->HasHandshakeHappened();

			return false;
		}
		tsqueue<owned_message<T>>& Incoming()
		{
			return m_qMessagesIn;
		}

	protected:
		asio::io_context m_context;
		asio::ssl::context m_sslContext;
        std::thread thrContext;
		std::unique_ptr<connection<T>> m_connection;

	private:
		tsqueue<owned_message<T>> m_qMessagesIn;
	};
}
