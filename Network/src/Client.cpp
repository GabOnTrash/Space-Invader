#include "../include/Common.hpp"
#include "../include/Client.hpp"
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

class Client : public network::clientInterface<MultiplayerPacketType>
{
private:
    std::unordered_map<uint32_t, PlayerInfo> otherPlayers;
    PlayerInfo thisPlayer;
public:
    std::unordered_map<uint32_t, PlayerInfo>& GetOtherPlayers() { return otherPlayers; }

    void SetPositionAndSend(float x, float y)
    {
        if (thisPlayer.x == x && thisPlayer.y == y)
            return;

        thisPlayer.x = x;
        thisPlayer.y = y;

        SendPosition();
    }
    void SendPosition()
    {
        network::message<MultiplayerPacketType> msg;
        msg.header.id = MultiplayerPacketType::PLAYER_MOVE;
        msg << thisPlayer;
        Send(msg);
    }
    void PollMessage()
    {
        if (!IsConnected()) return;

        while (!Incoming().empty())
        {
            auto msg = Incoming().pop_front().msg;

            switch (msg.header.id)
            {
                case MultiplayerPacketType::PLAYER_NOTIFY_ID:
                    msg >> thisPlayer.id;
                    std::cout << "[CLIENT] Getting player ID in __line__ 53: " << thisPlayer.id << std::endl;

                break;
                case MultiplayerPacketType::PLAYER_ADDED:
                    uint32_t id;
                    msg >> id;
                    std::cout << "[CLIENT] Storing player ID from msg in __line__ 59: " << id << std::endl;
                    if (id != thisPlayer.id)
                        otherPlayers[id] = {0.0f, 0.0f, id };
                break;
                case MultiplayerPacketType::PLAYER_MOVE:
                case MultiplayerPacketType::PLAYER_ADD_OTHERS:
                    PlayerInfo info;
                    msg >> info;
                    std::cout << "[CLIENT] Getting player struct from msg in __line__ 67: ID: " << info.id << " x: " << info.x << " y: " << info.y << std::endl;
                    if (info.id != thisPlayer.id)
                        otherPlayers[info.id] = info;
                break;
                case MultiplayerPacketType::PLAYER_REMOVED:
                    uint32_t deadID;
                    msg >> deadID;
                    std::cout << "[CLIENT] Getting id from msg to delete from map in __line__ 74: ID: " << deadID << std::endl;
                    otherPlayers.erase(deadID);
                break;

            }
        }
    }
};
