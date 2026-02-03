#include "../include/Common.hpp"
#include "../include/Server.hpp"
#include "../../Game/include/config/enums.hpp"

#include <unordered_map>

#pragma pack(push, 1)
struct PlayerInfo
{
    float x;
    float y;
    size_t id;
};
#pragma pack(pop)

class Server : public network::serverInterface<MultiplayerPacketType>
{
public:
    Server(uint16_t port) : network::serverInterface<MultiplayerPacketType>(port)
    {

    }
    virtual ~Server()
    {
    }
    std::vector<PlayerInfo> playersVector;

protected:
    virtual bool OnClientConnect(std::shared_ptr<network::connection<MultiplayerPacketType>> client) override
    {
        return true;
    }
    virtual void OnClientValidated(std::shared_ptr<network::connection<MultiplayerPacketType>> client) override
    {
        network::message<MultiplayerPacketType> msg;
        msg.header.id = MultiplayerPacketType::PLAYER_ADDED;

        // create a message containing only the player id
        size_t id = client->GetID();
        msg << id;
        MessageAllClient(msg, client);
    }
    virtual void OnClientDisconnect(std::shared_ptr<network::connection<MultiplayerPacketType>> client) override
    {
        network::message<MultiplayerPacketType> msg;
        msg.header.id = MultiplayerPacketType::PLAYER_REMOVED;

        size_t id = client->GetID();
        msg << id;
        MessageAllClient(msg, client);
    }
    virtual void OnMessage(std::shared_ptr<network::connection<MultiplayerPacketType>> client,
                           network::message<MultiplayerPacketType>& msg) override
    {
        PlayerInfo temp;

        switch (msg.header.id)
        {
        case MultiplayerPacketType::PLAYER_ADDED:
            break;
        case MultiplayerPacketType::PLAYER_MOVE:
        {
            // get the id connection, set the id, and send the struct to the others
            temp.id = client->GetID();
            msg.body.clear();
            msg << temp;
            MessageAllClient(msg, client);
        }
        break;
        case MultiplayerPacketType::PLAYER_SHOOT:
        case MultiplayerPacketType::PLAYER_DASH:
        default:
            break;
        }
    }
};