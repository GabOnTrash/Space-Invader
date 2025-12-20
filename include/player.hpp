#pragma once

#include "laser.hpp"
#include "specs.hpp"

class Player
{
public:

	Player();
	~Player();

	static void Init(); 
    static inline ByteMask byteMask;

	void Reset();

	void Update(float deltaT);
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
	void secooldownTimerLaserTimeToLive(int timeInMs)
	{
		laserTimeToLive = timeInMs;
    }
	bool activeBigLaser() 
	{ 
		return isBigLaserActive; 
	}

	void setTripleLaser(bool val) 
	{ 
		tripleLaser = val; 
	}
	void setReducedVel(bool val) 
	{ 
		reducedVel = val; 
	}
	 void useBigLaser(bool val) 
	 { 
		 isBigLaserActive = val; 
	 }
	BigLaser& getBigLaser() 
	{ 
		return bigLaser; 
	}
	Rectangle getBounds();
	Vector2 position{ 0, 0 };
	std::vector<Laser> lasers;

private:
	void Movement(float deltaT);
	void clearLaser();

	int vel = 0;
	int coolDown = 400;
	bool tripleLaser = false;
	bool reducedVel = false;
	bool isBigLaserActive = false;
	int laserTimeToLive = 2000; // in ms

	BigLaser bigLaser;
	Vector2 direction{ 0, 0 };
	Vector2 newPosition{ 0, 0 };

	Timer<> cooldownTimerLaser;

	static inline Texture2D texture = { 0 };
};
