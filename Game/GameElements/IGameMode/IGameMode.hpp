#pragma once

#include "../Player/Player.hpp"
#include "../MenuHandle/MenuHandle.hpp"
#include "../util/specs.hpp"
#include "../CollisionManager/CollisionManager.hpp"

class IGameMode
{
public:
    IGameMode(GameContext& ctx, MenuHandle& handle) : gameContext(ctx), player(gameContext), menuHandle(handle){};
    virtual ~IGameMode() = default;
    virtual void Init() = 0;           // Carica nemici, o connette al server
    virtual void Update(float dt) = 0; // Logica del frame
    virtual void Draw() = 0;           // Renderizzazione
    virtual void OnExit() = 0;         // Pulizia (es. disconnessione)

protected:
    CollisionManager cManager;
    GameContext& gameContext;
    MenuHandle& menuHandle;
    Player player;
};
