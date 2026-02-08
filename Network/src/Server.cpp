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
                std::cout << "[SERVER] Passing player ID in __line__ 49: " << idP << " with in player info: ID: " << info.id << " x: " << info.x << " y: " << info.y << std::endl;
                MessageClient(client, othersInfo);
            }

            playersMap[id] = { 0.0f, 0.0f, id };
        }
        // notify the id to the player
        network::message<MultiplayerPacketType> msgID;
        msgID.header.id = MultiplayerPacketType::PLAYER_NOTIFY_ID;
        msgID << id;
        std::cout << "[SERVER] Sending player his id with msg in __line__ 59: ID: " << id << std::endl;
        MessageClient(client, msgID);

        network::message<MultiplayerPacketType> msg;
        msg.header.id = MultiplayerPacketType::PLAYER_ADDED;
        std::cout << "[SERVER] Sending all other players new p. id from with msg in __line__ 64: ID: " << id << std::endl;
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
                std::cout << "[SERVER] Client disconnected: " << id << std::endl;
            }
            else return;
        }

        network::message<MultiplayerPacketType> msg;
        msg.header.id = MultiplayerPacketType::PLAYER_REMOVED;

        msg << id;
        MessageAllClient(msg, nullptr);
        std::cout << "[SERVER] Messaging all clients to delete player in __line__ 84: ID: " << id << std::endl;
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
            std::cout << "[SERVER] Getting player info to send to other clients in __line__ 96: ID: " << info.id << " x: " << info.x << " y: " << info.y << std::endl;
            {
                std::scoped_lock<std::mutex> lock(muxGame);
                playersMap[info.id] = info;
            }

            network::message<MultiplayerPacketType> outMsg;
            outMsg.header.id = MultiplayerPacketType::PLAYER_MOVE;
            outMsg << info;
            std::cout << "[SERVER] Sending player info to the other clients in __line__ 108: ID: " << info.id << " x: " << info.x << " y: " << info.y << std::endl;
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