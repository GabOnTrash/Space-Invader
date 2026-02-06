#pragma once

#include "../player.hpp"

struct GameContext;

class IGameMode
{
public:
    IGameMode(GameContext& ctx) : gameContext(ctx){};
    virtual ~IGameMode() = default;
    virtual void Init() = 0;           // Carica nemici, o connette al server
    virtual void Update(float dt) = 0; // Logica del frame
    virtual void Draw() = 0;           // Renderizzazione
    virtual void OnExit() = 0;         // Pulizia (es. disconnessione)
    virtual void Resume() = 0;

protected:
    Player player;
    GameContext& gameContext;
};
