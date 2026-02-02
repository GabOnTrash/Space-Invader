#include "../include/Common.hpp"
#include "../include/Client.hpp"
#include "../../Game/include/config/enums.hpp"

class Client : public network::clientInterface<MultiplayerPacketType>
{
  public:
    void SendPosition(float x, float y)
    {
        network::message<MultiplayerPacketType> msg;
        msg.header.id = MultiplayerPacketType::PLAYER_MOVE;
        msg << x << y;
        Send(msg);
    }
};
