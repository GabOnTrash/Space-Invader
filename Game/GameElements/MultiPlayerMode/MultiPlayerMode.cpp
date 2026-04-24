#include "MultiPlayerMode.hpp"

MultiPlayerMode::MultiPlayerMode(GameContext& ctx, MenuHandle& menu, std::string ip, uint16_t port)
    : IGameMode(ctx, menu), timeToConnect(5000)
{
    this->ip = ip;
    this->port = port;
}
MultiPlayerMode::~MultiPlayerMode()
{
    OnExit();
}

void MultiPlayerMode::Init()
{
    timeToConnect.active();
    player.Reset();

    if (!Connect(ip, port))
        *(gameContext.gameStatus) = GameState::ON_START_MENU;
}

void MultiPlayerMode::OnConnectionAccepted()
{
    timeToConnect.deactive();
    menuHandle.stillTryingConnecting = false;
    menuHandle.SetSizeAndId(otherPlayers.size() + 1, thisPlayer);
}

void MultiPlayerMode::Update(float dt)
{
    PollMessage();

    if (IsConnected() && HasHandshakeHappened())
    {
        player.Update(dt);
        SendPosition(player.position.x, player.position.y);
    }
    else
    {
        timeToConnect.update();
        if (!timeToConnect.isRunning)
            *(gameContext.gameStatus) = GameState::ON_START_MENU;
    }
}

void MultiPlayerMode::Draw()
{
    for (const auto& [_, p] : otherPlayers)
        DrawTexture(player.texture, p.x, p.y, WHITE);

    if (IsConnected())
        player.Draw();
}

void MultiPlayerMode::OnExit()
{
    Disconnect();
    SettingsManager::SaveData(gameContext.keyBindings, menuHandle);
}
