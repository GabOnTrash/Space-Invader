#pragma once

#include "specs.hpp"

#include "stars.hpp"
#include "player.hpp"
#include "meteor.hpp"
#include "explosion.hpp"
#include "modifier.hpp"
#include "heart.hpp"
#include "timer.hpp"
#include "MenuLayer.hpp"
#include "../../Network/src/Client.cpp"

class GameLayer
{
public:
    GameLayer() = default;
    GameLayer(std::shared_ptr<GameState> GameStatus, std::shared_ptr<MenuLayer> MenuSystem);
	~GameLayer();

    void UpdateSystem();
    std::vector<Heart>& GetHeartsArray();
    int& GetGameScore();
    void SetMaxHearts(int hearts);

    void Start();
    void StartMulti();
    void Restart();
    void Resume();
    void SetDiff();

    static void InitIcons();

private:
    void UpdateRunMenuLayer();
    void DrawRunMenuLayer();
    void UpdateElements();
    void UpdateTimers();
    void CreatePowerUp();
    void ClearEffects();
    void CheckAllCollisions();

    void DrawLinks() const;

    float deltaT = 0.0f;

    int GameScore = 0;
    int nMaxHearts = 0;
    int DiffPerModifiers = 0;
    int modifierType = 0;
    int meteorsDelay = 500;

    bool ElementsUpdating = true;

    Client client;

    std::shared_ptr<GameState> GameStatus;
    std::shared_ptr<MenuLayer> MenuSystem; 

	Star menuStars;
    Star runningStars;

    static inline Texture2D instagram = {};
    static inline Texture2D github = {};

    GameMode gameMode = SINGLEPLAYER;

    Player player;

    Timer<> meteorTimer;
    Timer<> modifierTimer;
    Timer<> tripleLaserTimer;
    Timer<> bigLaserTimer;
    Timer<> reducedVelTimer;
    Timer<> timerDelayResume;

    std::vector<Heart> heartsArray;
    std::vector<Meteor> meteors;
    std::vector<Explosion> explosions;
    std::vector<Modifier> modifiers;

    std::thread collisionThread;
    std::mutex collisionMutex;
    std::atomic<bool> runningCollision = true;
};