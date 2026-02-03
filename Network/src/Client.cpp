#include "../include/Common.hpp"
#include "../include/Client.hpp"
#include "../../Game/include/config/enums.hpp"

#pragma pack(push, 1)
struct PlayerInfo
{
    float x;
    float y;
    size_t id;
};
#pragma pack(pop)

class Client : public network::clientInterface<MultiplayerPacketType>
{
private:
    PlayerInfo thisPlayer;
public:
    std::vector<PlayerInfo> otherPlayers;

    void SetPositionAndSend(float& x, float& y)
    {
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
        if (IsConnected())
        {
            if (!Incoming().empty())
            {
                auto msg = Incoming().pop_front().msg;
                PlayerInfo temp;

                switch (msg.header.id)
                {
                    case MultiplayerPacketType::PLAYER_MOVE:
                        // passing the whole struct
                        msg >> temp;
                        for (auto& player : otherPlayers)
                        {
                            if (temp.id == player.id)
                            {
                                player = temp;
                            }
                        }
                    break;
                    case MultiplayerPacketType::PLAYER_ADDED:
                        // passing only the id
                        msg >> temp.id;
                        otherPlayers.push_back(temp);
                    break;
                    case MultiplayerPacketType::PLAYER_REMOVED:
                        // passing only the id
                        msg >> temp.id;
                        for (size_t i = 0; i < otherPlayers.size(); i++)
                        {
                            if (temp.id == otherPlayers.at(i).id)
                            {
                                otherPlayers.erase(otherPlayers.begin() + i);
                                return;
                            }
                        }
                    break;
                }
            }
        }
    }
};
