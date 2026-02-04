#include "../include/Common.hpp"
#include "../include/Server.hpp"

#include <map>

#include "../../Game/include/config/enums.hpp"

#include <unordered_map>

#pragma pack(push, 1)
struct PlayerInfo
{
    float x;
    float y;
    uint32_t id;
};
#pragma pack(pop)

class Server : public network::serverInterface<MultiplayerPacketType>
{
private:
    std::mutex muxGame;
    std::unordered_map<uint32_t, PlayerInfo> playersMap;
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
        return true;
    }
    virtual void OnClientValidated(std::shared_ptr<network::connection<MultiplayerPacketType>> client) override
    {
        uint32_t id = client->GetID();
        {
            std::scoped_lock<std::mutex> lock(muxGame);
            for (auto const& [idP, info] : playersMap)
            {
                // send all the other users data to the new player
                network::message<MultiplayerPacketType> othersInfo;
                othersInfo.header.id = MultiplayerPacketType::PLAYER_ADD_OTHERS;
                othersInfo << info;
                MessageClient(client, othersInfo);
            }

            playersMap[id] = { 0.0f, 0.0f, id };
        }
        // notify the id to the player
        network::message<MultiplayerPacketType> msgID;
        msgID.header.id = MultiplayerPacketType::PLAYER_NOTIFY_ID;
        msgID << id;
        MessageClient(client, msgID);

        network::message<MultiplayerPacketType> msg;
        msg.header.id = MultiplayerPacketType::PLAYER_ADDED;
        msg << id;
        MessageAllClient(msg, client);
    }
    virtual void OnClientDisconnect(std::shared_ptr<network::connection<MultiplayerPacketType>> client) override
    {
        std::scoped_lock<std::mutex> lock(muxGame);
        uint32_t id = client->GetID();

        if (playersMap.erase(id) > 0)
        {
            network::message<MultiplayerPacketType> msg;
            msg.header.id = MultiplayerPacketType::PLAYER_REMOVED;

            msg << id;
            MessageAllClient(msg, nullptr);
        }
    }
    virtual void OnMessage(std::shared_ptr<network::connection<MultiplayerPacketType>> client,
                           network::message<MultiplayerPacketType>& msg) override
    {
        switch (msg.header.id)
        {
        case MultiplayerPacketType::PLAYER_ADDED:
            break;
        case MultiplayerPacketType::PLAYER_MOVE:
        {
            std::scoped_lock<std::mutex> lock(muxGame);
            PlayerInfo info;
            msg >> info;
            info.id = client->GetID();

            playersMap[info.id] = info;
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