#pragma once

#include "../Laser/Laser.hpp"
#include "../util/specs.hpp"

struct GameContext;

class Player
{
public:
	Player(GameContext& ctx);
	~Player();

    static inline ByteMask byteMask;

	void Reset();
	void Update(float dt);
	void Draw();

	Vector2 GetPosition()
	{
		return position;
	}
	Texture2D& GetImage()
	{
		return Player::texture;
	}
    Timer<>& GetTimer()
    {
        return cooldownTimerLaser;
    }
	BigLaser& getBigLaser()
	{
		return bigLaser;
	}
	bool activeBigLaser()
	{
		return bigLaserTimer.isRunning;
	}
	void setLaserTimeToLive(int timeInMs);
	void setDashTimer(int timeInMs);
	Rectangle getBounds();
	void ActivateBigLaser();
	void ActivateTripleLaser();
	void ActivateReducedVelocity();
    void generateLaser(std::vector<Laser>& lasers);

	Vector2 position{ 0, 0 };
	bool wantToGenerateLaser	= false;

private:
    void Movement(float dt);
    void HandleInput();
    void StartDash(Vector2 dir);
    void UpdateDash(float dt);

	int vel				= 500;
	int coolDown		= 400;
	int laserTimeToLive		= 2000; // in ms

    bool isDashing = false;
	bool wantToDash = false;
    float dashTimer = 0.0f;
    float dashDuration = 0.2f;
    float dashMultiplier = 4.f;

    Vector2 dashDir = { 0, 0 };

	BigLaser bigLaser;
	Vector2 direction{ 0, 0 };
	Vector2 newPosition{ 0, 0 };

	Timer<> cooldownTimerLaser;
    Timer<> cooldownTimerDash;
	Timer<> bigLaserTimer;
	Timer<> reducedVelTimer;
	Timer<> tripleLaserTimer;
public:
	static inline Texture2D texture = {};
	GameContext& ctx;
};
