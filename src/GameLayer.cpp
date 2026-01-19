#include "GameLayer.hpp"

GameLayer::GameLayer(std::shared_ptr<GameState> GameStatus, std::shared_ptr<MenuLayer> MenuSystem)
    : MenuSystem(MenuSystem), GameStatus(GameStatus),
      meteorTimer(0, [this]() { meteors.emplace_back(); }, true, true),
      modifierTimer(6000, [this]() { CreatePowerUp(); }, true, true),
      timerDelayResume(1500, [this]() { ElementsUpdating = true; }, false, false),
      tripleLaserTimer(5000, [this]() { player.setTripleLaser(false); }),
      bigLaserTimer(6000, [this]() { player.useBigLaser(false); }),
      reducedVelTimer(5000, [this]() { player.setReducedVel(false); })
{
    meteors.reserve(30);
    explosions.reserve(10);
    modifiers.reserve(10);

    runningStars.InitStars(0);
    menuStars.InitStars(1);

    collisionThread = std::thread(
        [this]()
        {
            while (runningCollision)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                if (*this->GameStatus == RUNNING && ElementsUpdating)
                {
                    std::lock_guard<std::mutex> lock(collisionMutex);
                    CheckAllCollisions();
                }
            }
        });
}

GameLayer::~GameLayer()
{
    runningCollision = false;
    if (collisionThread.joinable())
        collisionThread.join();
}

void GameLayer::DrawLinks() const
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

void GameLayer::UpdateSystem()
{
    deltaT = GetFrameTime();

    switch (*GameStatus)
    {
    case START:
    {
        menuStars.updateStars(deltaT, 1);
        menuStars.DrawStars();
        DrawLinks();

        break;
    }
    case KILLED:
    {
        DrawRunMenuLayer();
        DrawRectangle(0, 0, ViewPort::BASE_WIDTH, ViewPort::BASE_HEIGHT, Fade(BLACK, 0.5f));
        
        break;
    }
    case RUNNING:
    {
        UpdateRunMenuLayer();
        DrawRunMenuLayer();

        heartsArray.empty() ? MenuSystem->GetRunningMenu()->activate("labelDanger")
                                : MenuSystem->GetRunningMenu()->deactive("labelDanger");
        !ElementsUpdating ? MenuSystem->GetRunningMenu()->activate("labelReady")
                            : MenuSystem->GetRunningMenu()->deactive("labelReady");

        break;
    }
    case PAUSED:
    {
        DrawRunMenuLayer();
        DrawRectangle(0, 0, ViewPort::BASE_WIDTH, ViewPort::BASE_HEIGHT, Fade(BLACK, 0.5f));

        break;
    }
    }
}

void GameLayer::UpdateRunMenuLayer()
{
    runningStars.updateStars(deltaT, 0);

    if (ElementsUpdating)
    {
        UpdateTimers();
        UpdateElements();
        player.Update(deltaT);
    }
    else
        timerDelayResume.update();
}
void GameLayer::DrawRunMenuLayer()
{
    runningStars.DrawStars();

    for (auto& Meteor : meteors)
        Meteor.Draw();

    for (auto& mod : modifiers)
        mod.Draw();

    for (auto& laser : player.lasers)
        laser.Draw();

    if (player.activeBigLaser())
        player.getBigLaser().Draw();

    player.Draw();

    for (int i = 0; i < heartsArray.size(); i++)
    {
        heartsArray[i].position = { ViewPort::BASE_WIDTH - (i + 1) * heartsArray[i].getBounds().width - 20, 20 };
        heartsArray[i].Draw();
    }
}

void GameLayer::UpdateElements()
{
    std::lock_guard<std::mutex> lock(collisionMutex);

    for (size_t i = 0; i < meteors.size();)
    {
        meteors[i].Update(deltaT);

        if (meteors[i].getBounds().y > ViewPort::BASE_HEIGHT + meteors[i].getBounds().height)
        {
            meteors[i] = meteors.back();
            meteors.pop_back();
        }
        else
            i++;
    }

    for (size_t i = 0; i < modifiers.size();)
    {
        modifiers[i].Update(deltaT);

        if (modifiers[i].getBounds().y > ViewPort::BASE_HEIGHT + modifiers[i].getBounds().height)
        {
            modifiers[i] = modifiers.back();
            modifiers.pop_back();
        }
        else
            i++;
    }

    for (size_t i = 0; i < explosions.size();)
    {
        explosions[i].Update(deltaT);

        if (explosions[i].end)
        {
            explosions[i] = explosions.back();
            explosions.pop_back();
        }
        else
            i++;
    }

    for (size_t i = 0; i < player.lasers.size();)
    {
        player.lasers[i].Update(deltaT);

        if (player.lasers[i].position.y < -50 || player.lasers[i].shouldDie())
        {
            player.lasers[i] = player.lasers.back();
            player.lasers.pop_back();
        }
        else
            i++;
    }
}
void GameLayer::UpdateTimers()
{
    meteorTimer.update();
    modifierTimer.update();

    tripleLaserTimer.update();
    bigLaserTimer.update();
    reducedVelTimer.update();
}

void GameLayer::CreatePowerUp()
{
    modifierType = GetRN<int>(0, DiffPerModifiers);
    modifiers.emplace_back(modifierType);
}

void GameLayer::ClearEffects()
{
    tripleLaserTimer.deactive();
    bigLaserTimer.deactive();
    reducedVelTimer.deactive();

    modifiers.clear();
    meteors.clear();
    explosions.clear();
    heartsArray.clear();

    player.Reset();
    GameScore = 0;
}
void GameLayer::CheckAllCollisions()
{
    // Collisioni meteors laser
    for (size_t i = 0; i < player.lasers.size();)
    {
        bool laserErased = false;
        for (size_t j = 0; j < meteors.size();)
        {
            Rectangle rLaser = player.lasers[i].getBounds();
            Rectangle rAst = meteors[j].getBounds();

            if (CheckCollisionRecs(rLaser, rAst))
            {
                if (Laser::byteMask.checkPixelCollision(Meteor::byteMask, player.lasers[i].position, meteors[j].position, GetCollisionRec(rLaser, rAst)))
                {
                    player.lasers.erase(player.lasers.begin() + i);

                    if (meteors[j].getState() == DAMAGED)
                    {
                        explosions.emplace_back(meteors[j].getBounds());

                        meteors[j] = meteors.back();
                        meteors.pop_back();

                        laserErased = true;
                        GameScore++;
                    }
                    else
                    {
                        meteors[j].NextState();
                    }
                    break;
                }
            }
            j++;
        }
        if (!laserErased)
            i++;
    }

    if (player.activeBigLaser())
    {
        for (size_t i = 0; i < meteors.size();)
        {
            Rectangle rLaser = player.getBigLaser().getBounds();
            Rectangle rAst = meteors[i].getBounds();

            if (CheckCollisionRecs(rLaser, rAst))
            {
                if (BigLaser::byteMask.checkPixelCollision(Meteor::byteMask, player.getBigLaser().position, meteors[i].position, GetCollisionRec(rLaser, rAst)))
                {
                    if (meteors[i].getState() == DAMAGED)
                    {
                        explosions.emplace_back(meteors[i].getBounds());
                        meteors[i] = meteors.back();
                        meteors.pop_back();
                        GameScore++;
                        continue;
                    }
                    else
                    {
                        meteors[i].NextState();
                    }
                }
            }

            i++;
        }
    }

    // Collisioni meteors player
    for (size_t i = 0; i < meteors.size();)
    {
        Rectangle rPlayer = player.getBounds();
        Rectangle rAsteroid = meteors[i].getBounds();

        if (CheckCollisionRecs(rPlayer, rAsteroid))
        {
            if (Player::byteMask.checkPixelCollision(Meteor::byteMask, player.position, meteors[i].position, GetCollisionRec(rPlayer, rAsteroid)))
            {
                if (heartsArray.size() == 0)
                {
                    *GameStatus = KILLED;
                    return;
                }
                else
                {
                    heartsArray.pop_back();
                    meteors[i] = meteors.back();
                    meteors.pop_back();
                    continue;
                }
            }
        }
        i++;
    }

    // Collisioni modifiers player
    for (size_t i = 0; i < modifiers.size();)
    {
        Rectangle rPlayer = player.getBounds();
        Rectangle rPot = modifiers[i].getBounds();

        if (CheckCollisionRecs(rPlayer, rPot))
        {
            if (Player::byteMask.checkPixelCollision(Modifier::byteMask, player.position, modifiers[i].position, GetCollisionRec(rPlayer, rPot)))
            {
                if (modifiers[i].modType == ModifierType::SLOWER)
                {
                    player.setReducedVel(true);
                    reducedVelTimer.active();
                }
                else if (modifiers[i].modType == ModifierType::LOSEHEART)
                {
                    if (heartsArray.empty())
                        *GameStatus = KILLED;
                    else
                        heartsArray.pop_back();
                }
                else if (modifiers[i].modType == ModifierType::TRIPLELASER)
                {
                    player.setTripleLaser(true);
                    tripleLaserTimer.active();
                }
                else if (modifiers[i].modType == ModifierType::GAINHEART)
                {
                    if (heartsArray.size() < nMaxHearts)
                        heartsArray.emplace_back();
                }
                else if (modifiers[i].modType == ModifierType::BIGLASER)
                {
                    player.useBigLaser(true);
                    bigLaserTimer.active();
                }

                modifiers[i] = modifiers.back();
                modifiers.pop_back();
                continue;
            }
        }

        i++;
    }
}
void GameLayer::Resume()
{
    explosions.clear();
    timerDelayResume.deactive();
    timerDelayResume.active();

    ElementsUpdating = false;
    MenuSystem->SetLayerGame();
}
void GameLayer::Restart()
{
    Start();
}
void GameLayer::Start()
{
    ClearEffects();

    switch (MenuSystem->GetGameDifficulty())
    {
    case EASY:
        GetHeartsArray().emplace_back();
        [[fallthrough]];
    case NORMAL:
        GetHeartsArray().emplace_back();
        [[fallthrough]];
    case HARD:
        GetHeartsArray().emplace_back();
        [[fallthrough]];
    default:
        break;
    }

    MenuSystem->SetLayerGame();
}
void GameLayer::SetDiff()
{
    SetMaxHearts(MenuSystem->GetGameDifficulty() - 2);
    player.setLaserTimeToLive((MenuSystem->GetGameDifficulty()) * 600); // 3.0 s, 2.4 s, 1.8 s
    player.setDashTimer(4000 / MenuSystem->GetGameDifficulty()); // 800 ms 1000 ms 1333 ms
    meteorTimer.duration = std::chrono::milliseconds(MenuSystem->GetGameDifficulty() * 100);
    DiffPerModifiers = MenuSystem->GetGameDifficulty() - 1;
}

std::vector<Heart>& GameLayer::GetHeartsArray()
{
    return heartsArray;
}
int& GameLayer::GetGameScore()
{
    return GameScore;
}
void GameLayer::SetMaxHearts(int hearts)
{
    nMaxHearts = hearts;
}

void GameLayer::InitIcons()
{
    if (ViewPort::loadedResources)
    {
        instagram = AssetsManager::GetTexture("instagram");
        github = AssetsManager::GetTexture("github");
    }
}
