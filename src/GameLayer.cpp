#include "GameLayer.hpp"

GameLayer::GameLayer(std::shared_ptr<GameState> GameStatus, std::shared_ptr<Interface> MenuSystem)
    : MenuSystem(MenuSystem), GameStatus(GameStatus),
      tAsteroidi(0, [this]() { asteroidi.emplace_back(); }, true, true),
      tPotenziamenti(6000, [this]() { CreatePowerUp(); }, true, true),
      tResume(1500, [this]() { ElementsUpdating = true; }, false, false),
      PotTriplo(5000, [this]() { giocatore.tripleLaser = false; }),
      PotContinuo(6000, [this]() { giocatore.continued = false; }),
      DpotLento(5000, [this]() { giocatore.reducedVel = false; })
{
    asteroidi.reserve(30);
    esplosioni.reserve(10);
    potenziamenti.reserve(10);

    stelleGioco.InizializzaStelle(0);
    stelleMenu.InizializzaStelle(1);

    collisionThread = std::thread(
        [this]()
        {
            while (runningCollision)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                if (*this->GameStatus == RUNNING && ElementsUpdating)
                {
                    std::lock_guard<std::mutex> lock(collisionMutex);
                    checkAllCollisions();
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

void GameLayer::UpdateGameStatus(float deltaT)
{
    this->deltaT = deltaT;

    switch (*GameStatus)
    {
    case START:
    {
        stelleMenu.AggiornaStelle(deltaT, 1);
        stelleMenu.DisegnaStelle();
        
        break;
    }
    case KILLED:
    {
        DrawRunInterface();
        DrawRectangle(0, 0, BASE_WIDTH, BASE_HEIGHT, Fade(BLACK, 0.3f));
        
        break;
    }
    case RUNNING:
    {
        UpdateRunInterface();
        DrawRunInterface();

        arrayCuori.size() == 0 ? MenuSystem->GetRunningMenu()->activate("labelDanger")
                                : MenuSystem->GetRunningMenu()->deactive("labelDanger");
        !ElementsUpdating ? MenuSystem->GetRunningMenu()->activate("labelReady")
                            : MenuSystem->GetRunningMenu()->deactive("labelReady");

        break;
    }
    case PAUSED:
    {
        DrawRunInterface();
        DrawRectangle(0, 0, BASE_WIDTH, BASE_HEIGHT, Fade(BLACK, 0.3f));

        break;
    }
    }
}

void GameLayer::UpdateRunInterface()
{
    stelleGioco.AggiornaStelle(deltaT, 0);

    if (ElementsUpdating)
    {
        UpdateTimers();
        UpdateElements();
        giocatore.Aggiorna(deltaT);
    }
    else
        tResume.update();
}
void GameLayer::DrawRunInterface()
{
    stelleGioco.DisegnaStelle();

    for (auto& asteroide : asteroidi)
        asteroide.Disegna();

    for (auto& potenziam : potenziamenti)
        potenziam.Disegna();

    for (auto& laser : giocatore.lasers)
        laser.Disegna();

    if (giocatore.continued)
        giocatore.bigLaser.Disegna();

    giocatore.Disegna();

    for (int i = 0; i < arrayCuori.size(); i++)
    {
        arrayCuori[i].posizione = { static_cast<float>(BASE_WIDTH - (i + 1) * arrayCuori[i].getWidth()), 20 };
        arrayCuori[i].Disegna();
    }
}


void GameLayer::UpdateElements()
{
    std::lock_guard<std::mutex> lock(collisionMutex);

    for (size_t i = 0; i < asteroidi.size();)
    {
        asteroidi[i].Aggiorna(deltaT);

        if (asteroidi[i].getBounds().y > 100 + BASE_HEIGHT)
        {
            asteroidi[i] = std::move(asteroidi.back());
            asteroidi.pop_back();
        }
        else
            i++;
    }

    for (size_t i = 0; i < potenziamenti.size();)
    {
        potenziamenti[i].Aggiorna(deltaT);

        if (potenziamenti[i].getBounds().y > 100 + BASE_HEIGHT)
        {
            potenziamenti[i] = std::move(potenziamenti.back());
            potenziamenti.pop_back();
        }
        else
            i++;
    }

    for (size_t i = 0; i < esplosioni.size();)
    {
        esplosioni[i].Aggiorna(deltaT);

        if (esplosioni[i].fine)
        {
            esplosioni[i] = std::move(esplosioni.back());
            esplosioni.pop_back();
        }
        else
            i++;
    }

    for (size_t i = 0; i < giocatore.lasers.size();)
    {
        giocatore.lasers[i].Aggiorna(deltaT);

        if (giocatore.lasers[i].posizione.y < -50 || giocatore.lasers[i].shouldDie())
        {
            giocatore.lasers[i] = std::move(giocatore.lasers.back());
            giocatore.lasers.pop_back();
        }
        else
            i++;
    }
}
void GameLayer::UpdateTimers()
{
    tAsteroidi.update();
    tPotenziamenti.update();

    PotTriplo.update();
    PotContinuo.update();
    DpotLento.update();
}

void GameLayer::CreatePowerUp()
{
    tipoPotenziamento = GetRN<int>(0, DiffPerPotenziamenti);
    potenziamenti.emplace_back(tipoPotenziamento);
}

void GameLayer::ClearEffects()
{
    PotTriplo.deactive();
    PotContinuo.deactive();
    DpotLento.deactive();

    potenziamenti.clear();
    asteroidi.clear();
    esplosioni.clear();
    arrayCuori.clear();

    giocatore.Reset();

    tAsteroidi.duration = std::chrono::milliseconds(400);

    GameScore = 0;
}
void GameLayer::checkAllCollisions()
{
    // Collisioni asteroidi laser
    for (size_t i = 0; i < giocatore.lasers.size();)
    {
        bool laserErased = false;
        for (size_t j = 0; j < asteroidi.size();)
        {
            Rectangle rLaser = giocatore.lasers[i].getBounds();
            Rectangle rAst = asteroidi[j].getBounds();

            if (CheckCollisionRecs(rLaser, rAst))
            {
                if (Laser::byteMask.checkPixelCollision(Asteroide::byteMask, giocatore.lasers[i].posizione, asteroidi[j].posizione, GetCollisionRec(rLaser, rAst)))
                {
                    giocatore.lasers.erase(giocatore.lasers.begin() + i);

                    if (asteroidi[j].getState() == DAMAGED)
                    {
                        esplosioni.emplace_back(asteroidi[j].getBounds());

                        asteroidi[j] = std::move(asteroidi.back());
                        asteroidi.pop_back();

                        laserErased = true;
                        GameScore++;
                    }
                    else
                    {
                        asteroidi[j].NextState();
                    }
                    break;
                }
            }
            j++;
        }
        if (!laserErased)
            i++;
    }

    if (giocatore.continued)
    {
        for (size_t i = 0; i < asteroidi.size();)
        {
            Rectangle rLaser = giocatore.bigLaser.getBounds();
            Rectangle rAst = asteroidi[i].getBounds();

            if (CheckCollisionRecs(rLaser, rAst))
            {
                if (BigLaser::byteMask.checkPixelCollision(Asteroide::byteMask, giocatore.bigLaser.posizione, asteroidi[i].posizione, GetCollisionRec(rLaser, rAst)))
                {
                    if (asteroidi[i].getState() == DAMAGED)
                    {
                        esplosioni.emplace_back(asteroidi[i].getBounds());
                        asteroidi[i] = std::move(asteroidi.back());
                        asteroidi.pop_back();
                        GameScore++;
                        continue;
                    }
                    else
                    {
                        asteroidi[i].NextState();
                    }
                }
            }

            i++;
        }
    }

    // Collisioni asteroidi giocatore
    for (size_t i = 0; i < asteroidi.size();)
    {
        Rectangle rPlayer = giocatore.getBounds();
        Rectangle rAsteroid = asteroidi[i].getBounds();

        if (CheckCollisionRecs(rPlayer, rAsteroid))
        {
            if (Astronave::byteMask.checkPixelCollision(Asteroide::byteMask, giocatore.posizione, asteroidi[i].posizione, GetCollisionRec(rPlayer, rAsteroid)))
            {
                if (arrayCuori.size() == 0)
                {
                    *GameStatus = KILLED;
                    return;
                }
                else
                {
                    arrayCuori.pop_back();
                    asteroidi[i] = std::move(asteroidi.back());
                    asteroidi.pop_back();
                    continue;
                }
            }
        }
        i++;
    }

    // Collisioni potenziamenti giocatore
    for (size_t i = 0; i < potenziamenti.size();)
    {
        Rectangle rPlayer = giocatore.getBounds();
        Rectangle rPot = potenziamenti[i].getBounds();

        if (CheckCollisionRecs(rPlayer, rPot))
        {
            if (Astronave::byteMask.checkPixelCollision(PowerUp::byteMask, giocatore.posizione, potenziamenti[i].posizione, GetCollisionRec(rPlayer, rPot)))
            {
                if (potenziamenti[i].tipoPotenziamento == "downgrade")
                {
                    giocatore.reducedVel = true;
                    DpotLento.active();
                }
                else if (potenziamenti[i].tipoPotenziamento == "depotCuore")
                {
                    if (arrayCuori.size() <= 0)
                        *GameStatus = KILLED;
                    else
                        arrayCuori.pop_back();
                }
                else if (potenziamenti[i].tipoPotenziamento == "triploLaser")
                {
                    giocatore.tripleLaser = true;
                    PotTriplo.active();
                }
                else if (potenziamenti[i].tipoPotenziamento == "potCuore")
                {
                    if (arrayCuori.size() < nMaxCuori)
                        arrayCuori.emplace_back();
                }
                else if (potenziamenti[i].tipoPotenziamento == "bigLaser")
                {
                    giocatore.continued = true;
                    PotContinuo.active();
                }

                potenziamenti[i] = std::move(potenziamenti.back());
                potenziamenti.pop_back();
                continue;
            }
        }

        i++;
    }
}
void GameLayer::Resume()
{
    esplosioni.clear();
    tResume.deactive();
    tResume.active();

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
        GetArrayCuori().emplace_back();
        [[fallthrough]];
    case NORMAL:
        GetArrayCuori().emplace_back();
        [[fallthrough]];
    case HARD:
        GetArrayCuori().emplace_back();
        [[fallthrough]];
    default:
        break;
    }

    MenuSystem->SetLayerGame();
}
void GameLayer::SetDiff()
{
    SetMaxCuori(MenuSystem->GetGameDifficulty() - 2);
    giocatore.setLaserTimeToLive((MenuSystem->GetGameDifficulty()) * 600); // 3.0 s, 2.4 s, 1.8 s
    tAsteroidi.duration = std::chrono::milliseconds(MenuSystem->GetGameDifficulty() * 100);
    DiffPerPotenziamenti = MenuSystem->GetGameDifficulty() - 1;
}

std::vector<Cuore>& GameLayer::GetArrayCuori()
{
    return arrayCuori;
}
int& GameLayer::GetGameScore()
{
    return GameScore;
}
void GameLayer::SetMaxCuori(int cuori)
{
    nMaxCuori = cuori;
}