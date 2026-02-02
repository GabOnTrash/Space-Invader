#include "../include/Common.hpp"
#include "../include/Server.hpp"
#include "../../Game/include/config/enums.hpp"

class Server : public network::serverInterface<MultiplayerPacketType>
{
  public:
    Server(uint16_t port) : network::serverInterface<MultiplayerPacketType>(port)
    {
    }
    virtual ~Server()
    {
    }

  protected:
    virtual bool OnClientConnect(std::shared_ptr<network::connection<MultiplayerPacketType>> client) override
    {
        network::message<MultiplayerPacketType> msg;
        msg.header.id = MultiplayerPacketType::SERVER_ACCEPT;

        std::string str = "New player added to the server! ID: " + std::to_string(client->GetID());
        const char* cstr = str.c_str();
        msg << cstr;
        this->MessageAllClient(msg, client);
        client->Send(msg);

        return true;
    }
    virtual void OnClientDisconnect(std::shared_ptr<network::connection<MultiplayerPacketType>> client) override
    {
        network::message<MultiplayerPacketType> msg;
        msg.header.id = MultiplayerPacketType::PLAYER_DISCONNECT;

        std::string str = "Player disconnected from the server! ID: " + std::to_string(client->GetID());
        const char* cstr = str.c_str();
        msg << cstr;
        this->MessageAllClient(msg, client);
    }
    virtual void OnMessage(std::shared_ptr<network::connection<MultiplayerPacketType>> client,
                           network::message<MultiplayerPacketType>& msg) override
    {
        switch (msg.header.id)
        {
        case MultiplayerPacketType::PLAYER_MOVE:
        {
            float y, x;
            msg >> y >> x;
            std::cout << "Player with ID: " << client->GetID() << " moved to: (" << x << ", " << y << ")\n";
        }
        break;
        case MultiplayerPacketType::PLAYER_SHOOT:
        case MultiplayerPacketType::PLAYER_DASH:
        default:
            break;
        }
    }
};