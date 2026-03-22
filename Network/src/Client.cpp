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
                    LOG_INFO("[Player #" + std::to_string(thisPlayer.id) + "] got notified of own ID");

                break;
                case MultiplayerPacketType::PLAYER_ADDED:
                    uint32_t id;
                    msg >> id;
                    LOG_INFO("[Player #" + std::to_string(thisPlayer.id) + "] got notified of [Player #" + std::to_string(id) + "]");
                    if (id != thisPlayer.id)
                        otherPlayers[id] = {0.0f, 0.0f, id };
                break;
                case MultiplayerPacketType::PLAYER_MOVE:
                case MultiplayerPacketType::PLAYER_ADD_OTHERS:
                    PlayerInfo info;
                    msg >> info;
                    LOG_INFO("[Player #" + std::to_string(thisPlayer.id) + "] retrieved player #" + std::to_string(info.id)
                        + " struct {" + std::to_string(info.x) + ", " + std::to_string(info.y) + "}");

                    if (info.id != thisPlayer.id)
                        otherPlayers[info.id] = info;
                break;
                case MultiplayerPacketType::PLAYER_REMOVED:
                    uint32_t deadID;
                    msg >> deadID;
                    LOG_INFO("[Player #" + std::to_string(thisPlayer.id) + "] notified deletion of player #" + std::to_string(deadID));
                    otherPlayers.erase(deadID);
                break;

            }
        }
    }
};
