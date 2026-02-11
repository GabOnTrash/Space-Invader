#include "SinglePlayerMode.hpp"

#include "../Game/Game.hpp"

SinglePlayerMode::SinglePlayerMode(GameContext& ctx, MenuHandle& handle)
    : IGameMode(ctx, handle), entityManager(ctx),
        meteorTimer(0, [this]() { CreateMeteor(); }, true, true),
        modifierTimer(6000, [this]() { CreateModifier(); }, true, true)
{
    SinglePlayerMode::Init();
}

SinglePlayerMode::~SinglePlayerMode()
{
}

void SinglePlayerMode::Update(float dt)
{
    this->dt = dt;

    if (*(gameContext.gameStatus) == GameState::RUNNING_GAME && menuHandle.GameShouldUpdate)
    {
        entityManager.Update(dt);
        player.Update(dt);

        if (player.wantToGenerateLaser)
        {
            player.generateLaser(entityManager.getContainer<Laser>()->getElements());
            player.wantToGenerateLaser = false;
        }

        cManager.ResolveAll(player,
                entityManager.getContainer<Meteor>()->getElements(),
            entityManager.getContainer<Laser>()->getElements(),
            entityManager.getContainer<Modifier>()->getElements(),
            entityManager.getContainer<Explosion>()->getElements(),
            GameScore, gameContext.gameStatus,  menuHandle.heartsArray, menuHandle.nMaxHearts);

        meteorTimer.update();
        modifierTimer.update();
    }
}

void SinglePlayerMode::Draw()
{
    entityManager.Draw();
    player.Draw();
}

void SinglePlayerMode::ClearEffects()
{
    player.Reset();

    entityManager.ClearAllContainers();
    menuHandle.heartsArray.clear();
    
    GameScore = 0;
}
void SinglePlayerMode::Init()
{
    ClearEffects();

    menuHandle.SetMenuSinglePlayer();
    menuHandle.UpdateDifficulty();

    player.setDashTimer(4000 / menuHandle.GameDifficulty); // 800 ms 1000 ms 1333 ms
    player.setLaserTimeToLive((menuHandle.GameDifficulty) * 600); // 3.0 s, 2.4 s, 1.8 s
    meteorTimer.duration = std::chrono::milliseconds(menuHandle.GameDifficulty * 100);
    modifierType = menuHandle.GameDifficulty - 1;
}
void SinglePlayerMode::OnExit()
{
    SettingsManager::SaveData(gameContext.keyBindings, menuHandle);
}
void SinglePlayerMode::CreateMeteor()
{
    auto x = GetRN<float>(-gameContext.renderer.BASE_WIDTH, gameContext.renderer.BASE_WIDTH * 2);
    auto y = GetRN<float>(-300, -100);
    entityManager.getContainer<Meteor>()->emplace(Vector2{x, y});
}
void SinglePlayerMode::CreateModifier()
{
    auto x = GetRN<float>(50, gameContext.renderer.BASE_WIDTH - 50);
    auto y = GetRN<float>(-300, -100);
    entityManager.getContainer<Modifier>()->emplace(Vector2{x, y}, GetRN<int>(0, modifierType));
}