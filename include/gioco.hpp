#pragma once

#include "astronave.hpp"
#include "asteroide.hpp"
#include "esplosione.hpp"
#include "powerup.hpp"
#include "stelle.hpp"
#include "cuore.hpp"
#include "interface.hpp"

#include "specs.hpp"
#include "settingsManager.hpp"


class Game
{
public:

    Game();
    ~Game();

    void Init();

    void Unload();

    void Run();

private:

    void UpdateGameStatus();

    void CreatePowerUp();
    void checkAllCollisions();
    void UpdateElements();

    void UpdateTimers();

    void UpdateRunInterface();
    void DrawRunInterface();

    void AudioManager();
    void LoadAssets();

    void ClearEffects();

    void Start();
    void Restart();
    void Resume();

    void SetDiff();

    void InitUI();

    void SaveSettingsFile();

    float deltaT = 0.0f;

    int GameScore = 0;
    int nMaxCuori = 0;
    int DiffPerPotenziamenti;
    int tipoPotenziamento = 0;
    int delayAsteroidi = 500;

    bool ElementsUpdating = true;

    GameState GameStatus = START;

    Music GameMusic;

    Astronave giocatore;
    Stella stelleMenu;
    Stella stelleGioco;

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

    Interface MenuSystem;

    std::thread collisionThread;
    std::mutex collisionMutex;
    std::atomic<bool> runningCollision = true;

    static inline Texture2D GameCursor = { 0 };
    static inline bool textureCaricata = false;
};