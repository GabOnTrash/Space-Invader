#include "MultiPlayerMode.hpp"

MultiPlayerMode::MultiPlayerMode(GameContext& ctx, MenuHandle& menu, std::string ip, uint16_t port)
    : IGameMode(ctx, menu)
{
    this->ip = ip;
    this->port = port;
}
MultiPlayerMode::~MultiPlayerMode()
{
}

void MultiPlayerMode::Init()
{
    Logger::Get().Init("Server_Log.txt");
    Connect(ip, port);
}

void MultiPlayerMode::Update(float dt)
{
    PollMessage();
    if (!IsConnected())
        *(gameContext.gameStatus) = GameState::ON_IP_MENU;
    SetPositionAndSend(player.position.x, player.position.y);
}

void MultiPlayerMode::Draw()
{
}

void MultiPlayerMode::OnExit()
{
    Disconnect();
    SettingsManager::SaveData(gameContext.keyBindings, menuHandle);
}
