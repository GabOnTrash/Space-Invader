#include "MultiPlayerMode.hpp"

MultiPlayerMode::MultiPlayerMode(GameContext& ctx, MenuHandle& menu)
    : IGameMode(ctx, menu)
{
    value = 7;
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
    PollMessage();

    //SetPositionAndSend(player.position.x, player.position.y);
}

void MultiPlayerMode::Draw()
{
}

void MultiPlayerMode::OnExit()
{
    Disconnect();
    SettingsManager::SaveData(gameContext.keyBindings, menuHandle);
}