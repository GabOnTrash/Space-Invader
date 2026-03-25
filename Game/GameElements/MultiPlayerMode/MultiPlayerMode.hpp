#pragma once
#include "../IGameMode/IGameMode.hpp"
#include "../../Network/src/Client.cpp"

class MultiPlayerMode : public IGameMode, public Client
{
public:
    MultiPlayerMode(GameContext& ctx, MenuHandle& menu, std::string ip, uint16_t port);
    ~MultiPlayerMode();

    void Init() override;
    void Update(float dt) override;
    void Draw() override;
    void OnExit() override;

private:
    std::string ip;
    uint16_t port;
    Timer<> timeToConnect;
};