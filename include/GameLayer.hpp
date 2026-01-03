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

class GameLayer
{
public:
    GameLayer() = default;
    GameLayer(std::shared_ptr<GameState> GameStatus, std::shared_ptr<MenuLayer> MenuSystem);
	~GameLayer();

	void UpdateSystem();

    void UpdateRunMenuLayer();
    void DrawRunMenuLayer();
    void UpdateElements();
    void UpdateTimers();
    void CreatePowerUp();
    void ClearEffects();
    void CheckAllCollisions();

    void Start();
    void Restart();
    void Resume();
    void SetDiff();

    std::vector<Heart>& GetHeartsArray();
    int& GetGameScore();
    void SetMaxHearts(int hearts);
   
private:
    float deltaT = 0.0f;

    int GameScore = 0;
    int nMaxHearts = 0;
    int DiffPerModifiers = 0;
    int modifierType = 0;
    int meteorsDelay = 500;

    bool ElementsUpdating = true;

    std::shared_ptr<GameState> GameStatus;
    std::shared_ptr<MenuLayer> MenuSystem; 

	Star menuStars;
    Star runningStars;

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