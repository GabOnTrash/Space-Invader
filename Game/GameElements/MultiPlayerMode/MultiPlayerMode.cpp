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
    if (!Connect(ip, port))
        *(gameContext.gameStatus) = GameState::ON_START_MENU;

    timeToConnect.active();
    player.Reset();
}

void MultiPlayerMode::Update(float dt)
{
    PollMessage();

    if (!IsConnected())
        timeToConnect.update();

    if (IsConnected())
    {
        menuHandle.stillTryingConnecting = false;
        menuHandle.SetSizeAndId(otherPlayers.size() + 1, thisPlayer);

        player.Update(dt);
        timeToConnect.deactive();
        SendPosition(player.position.x, player.position.y);
    }
    else if (!timeToConnect.isRunning && !IsConnected())
        *(gameContext.gameStatus) = GameState::ON_START_MENU;
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
