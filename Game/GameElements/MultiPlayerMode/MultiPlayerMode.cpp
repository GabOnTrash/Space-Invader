#include "MultiPlayerMode.hpp"

MultiPlayerMode::MultiPlayerMode(GameContext& ctx, MenuHandle& menu)
    : IGameMode(ctx, menu)
{
    value = 7;
    Init();
}
MultiPlayerMode::~MultiPlayerMode()
{
}

void MultiPlayerMode::Init()
{
    Connect("127.0.0.1", 56000);
}

void MultiPlayerMode::Update(float dt)
{
}

void MultiPlayerMode::Draw()
{
}

void MultiPlayerMode::OnExit()
{
    this->Disconnect();
    SettingsManager::SaveData(gameContext.keyBindings, menuHandle);
}
