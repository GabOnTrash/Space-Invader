#include "MultiPlayerMode.hpp"

MultiPlayerMode::MultiPlayerMode(GameContext& ctx, MenuHandle& menu, std::string ip, uint16_t port)
    : IGameMode(ctx, menu), timeToConnect(5000)
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
    if (!Connect(ip, port))
        *(gameContext.gameStatus) = GameState::ON_IP_MENU;

    timeToConnect.active();
}

void MultiPlayerMode::Update(float dt)
{
    PollMessage();
    timeToConnect.update();

    if (timeToConnect.isRunning)
        std::cout << "Trying to connect\n";

    if (!timeToConnect.isRunning && !IsConnected())
    {
        *(gameContext.gameStatus) = GameState::ON_START_MENU;
        menuHandle.BackToMainMenu();
    }

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
