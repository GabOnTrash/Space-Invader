#include "MultiPlayerMode.hpp"

MultiPlayerMode::MultiPlayerMode(GameContext& ctx, MenuHandle& menu, std::string ip, uint16_t port)
    : IGameMode(ctx, menu), timeToConnect(5000)
{
    this->ip = ip;
    this->port = port;

    if (ip == "..." && port == 0)
    {
        std::string p;
        std::ifstream ifile("auth/address.txt");
        std::getline(ifile, this->ip);
        std::getline(ifile, p);
        this->port = std::stoi(p.c_str());
    }
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
}
void MultiPlayerMode::SetSelfID(uint32_t id)
{
    menuHandle.SetID(id);
}
void MultiPlayerMode::SetGroupSize(size_t gs)
{
    menuHandle.SetSize(gs);
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

    if (IsConnected() && HasHandshakeHappened())
        player.Draw();
}

void MultiPlayerMode::OnExit()
{
    Disconnect();
    SettingsManager::SaveData(gameContext.keyBindings, menuHandle);
}
