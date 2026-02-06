#include "SinglePlayerMode.hpp"

#include "Game/Game.hpp"

SinglePlayerMode::SinglePlayerMode(GameContext& ctx)
    : IGameMode(ctx),
        meteorTimer(0, [this]() { meteors.emplace_back(); }, true, true),
        modifierTimer(6000, [this]() { SpawnModifier(); }, true, true)
{
    meteors.reserve(30);
    explosions.reserve(10);
    modifiers.reserve(10);
    
    Start();
}

SinglePlayerMode::~SinglePlayerMode()
{
}

void SinglePlayerMode::Update(float dt)
{
    this->dt = dt;

    DrawGamePlay();
    if (*(gameContext.gameStatus) == GameState::RUNNING_GAME)
    {
        UpdateGamePlay();
        cManager.ResolveAll(player,
            meteors, lasers, modifiers, explosions,
            GameScore, gameContext.gameStatus, heartsArray, nMaxHearts);
    }
}

void SinglePlayerMode::UpdateGamePlay()
{
    UpdateTimers();
    UpdateElements();
}
void SinglePlayerMode::DrawGamePlay()
{
    for (auto& mt : meteors)
        mt.Draw();

    for (auto& mod : modifiers)
        mod.Draw();

    for (auto& ls : lasers)
        ls.Draw();

    if (player.activeBigLaser())
        player.getBigLaser().Draw();

    player.Draw();
}

void SinglePlayerMode::UpdateElements()
{
    for (size_t i = 0; i < meteors.size();)
    {
        meteors[i].Update(dt);

        if (meteors[i].getBounds().y > gameContext.BASE_WIDTH + meteors[i].getBounds().height)
        {
            meteors[i] = meteors.back();
            meteors.pop_back();
        }
        else i++;
    }
    for (size_t i = 0; i < modifiers.size();)
    {
        modifiers[i].Update(dt);

        if (modifiers[i].getBounds().y > gameContext.BASE_HEIGHT + modifiers[i].getBounds().height)
        {
            modifiers[i] = modifiers.back();
            modifiers.pop_back();
        }
        else i++;
    }
    for (size_t i = 0; i < explosions.size();)
    {
        explosions[i].Update(dt);

        if (explosions[i].end)
        {
            explosions[i] = explosions.back();
            explosions.pop_back();
        }
        else i++;
    }
    for (size_t i = 0; i < lasers.size();)
    {
        lasers[i].Update(dt);

        if (lasers[i].position.y < -50 || lasers[i].shouldDie())
        {
            lasers[i] = lasers.back();
            lasers.pop_back();
        }
        else i++;
    }

    player.Update(dt);
}
void SinglePlayerMode::UpdateTimers()
{
    meteorTimer.update();
    modifierTimer.update();
    
    // Updating delegated to player 
}
void SinglePlayerMode::SpawnModifier()
{
    modifierType = GetRN<int>(0, DiffPerModifiers);
    modifiers.emplace_back(modifierType);
}

void SinglePlayerMode::ClearEffects()
{
    player.Reset();

    explosions.clear();
    modifiers.clear();
    meteors.clear();
    heartsArray.clear();
    
    GameScore = 0;
}
void SinglePlayerMode::Resume()
{
    explosions.clear();
    *(gameContext.gameStatus) = GameState::RUNNING_GAME;
}
void SinglePlayerMode::Start()
{
    ClearEffects();

    switch (gameContext.difficulty)
    {
    case EASY:
        heartsArray.emplace_back();
        [[fallthrough]];
    case NORMAL:
        heartsArray.emplace_back();
        [[fallthrough]];
    case HARD:
        heartsArray.emplace_back();
        [[fallthrough]];
    default:
        break;
    }

    nMaxHearts = gameContext.difficulty - 2;
    player.setDashTimer(4000 / gameContext.difficulty); // 800 ms 1000 ms 1333 ms
    player.setLaserTimeToLive((gameContext.difficulty) * 600); // 3.0 s, 2.4 s, 1.8 s
    meteorTimer.duration = std::chrono::milliseconds(gameContext.difficulty * 100);
    DiffPerModifiers = gameContext.difficulty - 1;
    // delegate MenuSystem->SetLayerGame(gameMode);
}

// on clear effect
/* tripleLaserTimer.deactive();
bigLaserTimer.deactive();
reducedVelTimer.deactive();
*/

// on resume, delegated to the menu
/*timerDelayResume.deactive();
    timerDelayResume.active();*/

/*
 // delegate to menu timerDelayResume.update();
for (auto const& [id, playerInfo] : client.GetOtherPlayers())
        DrawTexture(Player::texture, static_cast<int>(playerInfo.x), static_cast<int>(playerInfo.y), WHITE);

/*for (int i = 0; i < heartsArray.size(); i++)
    {
        heartsArray[i].position = { BASE_WIDTH - (i + 1) * heartsArray[i].getBounds().width - 20, 20 };
        heartsArray[i].Draw();
    }

*void SinglePlayerMode::StartMulti()
{
    gameMode = MULTIPLAYER;
    client.Connect("127.0.0.1", 54000);

    MenuSystem->SetLayerGame(gameMode);
}


void SinglePlayerMode::InitIcons()
{
    if (ViewPort::loadedResources)
    {
        instagram = AssetsManager::GetTexture("instagram");
        github = AssetsManager::GetTexture("github");
    }
}
*/
// DrawLinks();
// DrawRectangle(0, 0, ViewPort::BASE_WIDTH, ViewPort::BASE_HEIGHT, Fade(BLACK, 0.5f));
/* delegate to menu
        heartsArray.empty() && gameMode == SINGLEPLAYER
            ? MenuSystem->GetRunningMenu()->activate("labelDanger")
            : MenuSystem->GetRunningMenu()->deactive("labelDanger");

        !ElementsUpdating && gameMode == SINGLEPLAYER
            ? MenuSystem->GetRunningMenu()->activate("labelReady")
            : MenuSystem->GetRunningMenu()->deactive("labelReady");

        gameMode == SINGLEPLAYER
            ? MenuSystem->GetRunningMenu()->activate("labelScore")
            : MenuSystem->GetRunningMenu()->deactive("labelScore");
        */
/* delegate to the menu
void SinglePlayerMode::DrawLinks() const
{
    Vector2 mousePos = GetMousePosition();
    mousePos.x = (mousePos.x - ViewPort::offsetX) / ViewPort::scale;
    mousePos.y = (mousePos.y - ViewPort::offsetY) / ViewPort::scale;

    DrawTexture(instagram, ViewPort::BASE_WIDTH - instagram.width * 2, ViewPort::BASE_HEIGHT - instagram.height * 2, WHITE);
    DrawTexture(github, ViewPort::BASE_WIDTH - github.width * 3.5, ViewPort::BASE_HEIGHT - github.height * 2, WHITE);

    if (CheckCollisionPointRec(mousePos,
        Rectangle{static_cast<float>(ViewPort::BASE_WIDTH - instagram.width * 2),
            static_cast<float>(ViewPort::BASE_HEIGHT - instagram.height * 2),
            static_cast<float>(instagram.width), static_cast<float>(instagram.height) }) && IsMouseButtonPressed(0))
    {
        OpenURL("https://www.instagram.com/_gabrielearmenise");
    }

    if (CheckCollisionPointRec(mousePos,
        Rectangle{static_cast<float>(ViewPort::BASE_WIDTH - github.width * 3.5),
            static_cast<float>(ViewPort::BASE_HEIGHT - github.height * 2.5),
            static_cast<float>(github.width), static_cast<float>(github.height) }) && IsMouseButtonPressed(0))
    {
        OpenURL("https://www.github.com/GabOnTrash");
    }
}
*/
