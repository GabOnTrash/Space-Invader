#include "MultiPlayerMode.hpp"

MultiPlayerMode::MultiPlayerMode(GameContext& ctx)
    : IGameMode(ctx)
{
}

void MultiPlayerMode::OnExit()
{
    this->Disconnect();
}
