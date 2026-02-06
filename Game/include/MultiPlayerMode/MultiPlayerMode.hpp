#pragma once
#include "../IGameMode/IGameMode.hpp"
#include "../../Network/src/Client.cpp"

class MultiPlayerMode : public IGameMode, public Client
{
public:
    MultiPlayerMode(GameMode& ctx);
    ~MultiPlayerMode();

    void OnExit() override;
private:

};