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
protected:
    virtual void SetSelfID(uint32_t id) {}
    virtual void SetGroupSize(size_t gs) {}
    virtual void OnConnectionAccepted() {}

    std::unordered_map<uint32_t, PlayerInfo> otherPlayers;
    uint32_t thisPlayer = -1;

public:
    std::unordered_map<uint32_t, PlayerInfo>& GetOtherPlayers() { return otherPlayers; }

    void SendPosition(float x, float y)
    {
        if (thisPlayer == -1) return;

        network::message<MultiplayerPacketType> msg;
        msg.header.id = MultiplayerPacketType::PLAYER_MOVE;
        msg << PlayerInfo{ x, y, thisPlayer };
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
                {
                    msg >> thisPlayer;
                    SetSelfID(thisPlayer);
                    LOG_INFO_FILE("[Player #" + std::to_string(thisPlayer) + "] got notified of own ID");
                }
                break;
                case MultiplayerPacketType::PLAYER_ADDED:
                {
                    uint32_t id;
                    msg >> id;
                    LOG_INFO_FILE("[Player #" + std::to_string(thisPlayer) + "] got notified of [Player #" + std::to_string(id) + "]");
                    if (id != thisPlayer)
                        otherPlayers[id] = {0.0f, 0.0f, id };
                    OnConnectionAccepted();
                }
                break;
                case MultiplayerPacketType::PLAYER_MOVE:
                case MultiplayerPacketType::PLAYER_ADD_OTHERS:
                {
                    PlayerInfo info;
                    msg >> info;

                    if (info.id != thisPlayer)
                        otherPlayers[info.id] = info;

                    SetGroupSize(otherPlayers.size());
                }
                break;
                case MultiplayerPacketType::PLAYER_REMOVED:
                {
                    uint32_t id;
                    msg >> id;
                    LOG_INFO_EVERYWHERE("[Player #" + std::to_string(thisPlayer) + "] got notified of deletion of player #" + std::to_string(id));
                    otherPlayers.erase(id);
                }
                break;
                default: ;
            }
        }
    }
};
