#pragma once
#include "Common.hpp"

namespace network
{
	template<typename T>
	struct messageHeader
	{
		T id{};
		uint32_t size = 0;
	};

	template <typename T>
	struct message
	{
		messageHeader<T> header{};
		std::vector<uint8_t> body;

		[[nodiscard]] size_t size() const
		{
            return sizeof(messageHeader<T>) + body.size();
		}

		friend std::ostream& operator << (std::ostream& os, const message<T>& msg)
		{
			os << "ID: " << int(msg.header.id) << " Size: " << msg.header.size;
			return os;
		}

		template <typename dataType>
		friend message<T>& operator << (message<T>& msg, dataType& data)
		{
            static_assert(std::is_standard_layout<dataType>::value, "Data is too complex to be pushed into the vector.");
			static_assert(std::is_trivially_copyable_v<dataType>, "Data is not trivially copyable (ex. strings vectors or pointers)");

			size_t start = msg.body.size();

			msg.body.resize(msg.body.size() + sizeof(dataType));
            std::memcpy(msg.body.data() + start, &data, sizeof(dataType));
			// appending at the end of the body vector the additional data with his size
			
			msg.header.size = msg.body.size();

			return msg;
		}
		
		template <typename dataType>
		friend message<T>& operator >> (message<T>& msg, dataType& data)
		{
			static_assert(std::is_standard_layout<dataType>::value, "Data is too complex to be pushed out of the vector.");
			static_assert(std::is_trivially_copyable_v<dataType>, "Data is not trivially copyable (ex. strings vectors or pointers)");

			size_t end = msg.body.size() - sizeof(dataType);

			std::memcpy(&data, msg.body.data() + end, sizeof(dataType));
			msg.body.resize(end);

			msg.header.size = msg.body.size();

			return msg;
		}
	};

	template <typename T>
	class connection;

	template <typename T>
	struct owned_message
	{
		std::shared_ptr<connection<T>> remote = nullptr;
		message<T> msg;

		friend std::ostream& operator << (std::ostream& os, const owned_message<T>& msg)
		{
			os << msg.msg;
			return os;
		}
	};
}
