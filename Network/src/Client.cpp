#include "../include/Common.hpp"
#include "../include/Client.hpp"
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
            uint32_t targetID;

            switch (msg.header.id)
            {
                case MultiplayerPacketType::PLAYER_NOTIFY_ID:
                    msg >> thisPlayer.id;
                break;
                case MultiplayerPacketType::PLAYER_ADDED:
                    msg >> targetID;
                    if (targetID != thisPlayer.id)
                        otherPlayers[targetID] = {0.0f, 0.0f, targetID };
                break;
                case MultiplayerPacketType::PLAYER_ADD_OTHERS:
                    PlayerInfo info;
                    msg >> info;
                    if (info.id != thisPlayer.id)
                        otherPlayers[info.id] = info;
                break;
                case MultiplayerPacketType::PLAYER_REMOVED:
                    msg >> targetID;
                    otherPlayers.erase(targetID);
                break;
                case MultiplayerPacketType::PLAYER_MOVE:
                    PlayerInfo infoM;
                    msg >> infoM;
                    if (infoM.id != thisPlayer.id)
                        otherPlayers[infoM.id] = infoM;
                break;
            }
        }
    }
};
