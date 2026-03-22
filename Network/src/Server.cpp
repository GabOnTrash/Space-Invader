#include "../include/Common.hpp"
#include "../include/Server.hpp"

#include <map>

#include "../../Game/GameElements/util/enums.hpp"

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

    void ListPlayers()
    {
        for (const auto& [_, info] : playersMap)
            LOG_INFO("[Player #" + std::to_string(info.id) + "]: {" + std::to_string(info.x) + ", " + std::to_string(info.y) + "}");
    }
    void KickPlayer(uint32_t id)
    {
        for (auto& conn : m_deqConnections)
        {
            if (id == conn->GetID())
                conn->Disconnect();
        }
    };

protected:
    bool OnClientConnect(std::shared_ptr<network::connection<MultiplayerPacketType>> client) override
    {
        return true;
    }
    void OnClientValidated(std::shared_ptr<network::connection<MultiplayerPacketType>> client) override
    {
        uint32_t id = client->GetID();
        {
            std::scoped_lock<std::mutex> lock(muxGame);
            for (const auto& [idP, info] : playersMap)
            {
                // send all the other users data to the new player
                network::message<MultiplayerPacketType> othersInfo;
                othersInfo.header.id = MultiplayerPacketType::PLAYER_ADD_OTHERS;
                othersInfo << info;
                LOG_INFO("[SERVER] Passing to player #" + std::to_string(id) + " info of player #"
                    + std::to_string(info.id) + " {" + std::to_string(info.x) + ", " + std::to_string(info.y) + "}");
                MessageClient(client, othersInfo);
            }

            playersMap[id] = { 0.0f, 0.0f, id };
        }
        // notify the id to the player
        network::message<MultiplayerPacketType> msgID;
        msgID.header.id = MultiplayerPacketType::PLAYER_NOTIFY_ID;
        msgID << id;
        LOG_INFO("[SERVER] Sending player #" + std::to_string(id) + " own id");
        MessageClient(client, msgID);

        network::message<MultiplayerPacketType> msg;
        msg.header.id = MultiplayerPacketType::PLAYER_ADDED;
        LOG_INFO("[SERVER] Sending player #" + std::to_string(id) + " infos to all");
        msg << id;
        MessageAllClient(msg, client);
    }
    void OnClientDisconnect(std::shared_ptr<network::connection<MultiplayerPacketType>> client) override
    {
        uint32_t id = client->GetID();
        {
            std::scoped_lock<std::mutex> lock(muxGame);
            if (playersMap.find(id) != playersMap.end())
            {
                playersMap.erase(id);
                LOG_INFO("[SERVER] Player #" + std::to_string(id) + " disconnected");
            }
            else return;
        }

        network::message<MultiplayerPacketType> msg;
        msg.header.id = MultiplayerPacketType::PLAYER_REMOVED;
        msg << id;

        MessageAllClient(msg, nullptr);
        LOG_INFO("[SERVER] Messaging all players to delete player #" + std::to_string(id));
    }
    void OnMessage(std::shared_ptr<network::connection<MultiplayerPacketType>> client,
                           network::message<MultiplayerPacketType>& msg) override
    {
        switch (msg.header.id)
        {
        case MultiplayerPacketType::PLAYER_ADDED:
            break;
        case MultiplayerPacketType::PLAYER_MOVE:
        {
            PlayerInfo info;
            msg >> info;

            info.id = client->GetID();
            LOG_INFO("[SERVER] Retrieving/Sending info from player #" + std::to_string(info.id) + "{ " + std::to_string(info.x) + ", " + std::to_string(info.y) + "}");
            {
                std::scoped_lock<std::mutex> lock(muxGame);
                playersMap[info.id] = info;
            }

            network::message<MultiplayerPacketType> outMsg;
            outMsg.header.id = MultiplayerPacketType::PLAYER_MOVE;
            outMsg << info;
            MessageAllClient(outMsg, client);
        }
        break;
        case MultiplayerPacketType::PLAYER_SHOOT:
        case MultiplayerPacketType::PLAYER_DASH:
        default:
            break;
        }
    }
};

