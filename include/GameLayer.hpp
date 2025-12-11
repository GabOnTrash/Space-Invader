#pragma once

#include "specs.hpp"

#include "stelle.hpp"
#include "astronave.hpp"
#include "asteroide.hpp"
#include "esplosione.hpp"
#include "powerup.hpp"
#include "cuore.hpp"
#include "timer.hpp"
#include "interface.hpp"

class GameLayer
{
public:
    GameLayer() = default;
    GameLayer(GameState* GameStatus, Interface* MenuSystem);
	~GameLayer();

	void UpdateGameStatus(float deltaT);

    void UpdateRunInterface();
    void DrawRunInterface();
    void UpdateElements();
    void UpdateTimers();
    void CreatePowerUp();
    void ClearEffects();
    void checkAllCollisions();

    void Start();
    void Restart();
    void Resume();

    void SetDiff();

    std::vector<Cuore>& GetArrayCuori();

    int& GetGameScore();
    
    void SetMaxCuori(int cuori);
    

private:

    float deltaT = 0.0f;

    int GameScore = 0;
    int nMaxCuori = 0;
    int DiffPerPotenziamenti;
    int tipoPotenziamento = 0;
    int delayAsteroidi = 500;

    bool ElementsUpdating = true;

    GameState* GameStatus;

	Stella stelleMenu;
    Stella stelleGioco;

    Interface* MenuSystem; 
	
    Astronave giocatore;

    Timer<> tAsteroidi;
    Timer<> tPotenziamenti;
    Timer<> PotTriplo;
    Timer<> PotContinuo;
    Timer<> DpotLento;
    Timer<> tResume;

    std::vector<Cuore> arrayCuori;
    std::vector<Asteroide> asteroidi;
    std::vector<Esplosione> esplosioni;
    std::vector<PowerUp> potenziamenti;

    std::thread collisionThread;
    std::mutex collisionMutex;
    std::atomic<bool> runningCollision = true;
};