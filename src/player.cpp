#include "player.hpp"

Player::Player()
	: cooldownTimerLaser(coolDown)
{
	Reset();
}
Player::~Player()
{
}

void Player::Init()
{
	if (loadedResources)
	{
		texture = AssetsManager::GetTexture("player");
        byteMask.loadFromImage(PATH_BYTEMASK_PLAYER);
	}
}

void Player::Reset()
{
	lasers.clear();
	lasers.reserve(10);

	position.x = ((BASE_WIDTH / 2.0f) - (texture.width / 2.0f));
	position.y = (BASE_HEIGHT / 2.0f);
	vel = 250;

	cooldownTimerLaser.deactive();

	tripleLaser = false;
	reducedVel = false;
	isBigLaserActive = false;
	clearLaser();
}

void Player::Draw()
{
    DrawTexture(texture, position.x, position.y, WHITE);
}

void Player::Update(float deltaT)
{
	Movement(deltaT);
	cooldownTimerLaser.update();
}

void Player::clearLaser()
{
	lasers.clear();
}

void Player::Movement(float deltaT)
{
	if (reducedVel) 
		vel = 150;
	
	else 
		vel = 250;

	// Input direction
	if (IsKeyDown(KeyBinds.KeyUP)) direction.y -= 1;
	if (IsKeyDown(KeyBinds.KeyDOWN)) direction.y += 1;
	if (IsKeyDown(KeyBinds.KeyLEFT)) direction.x -= 1;
	if (IsKeyDown(KeyBinds.KeyRIGHT)) direction.x += 1;

	if (sqrtf(direction.x * direction.x + direction.y * direction.y) != 0)
	{
		direction.x /= sqrtf(direction.x * direction.x + direction.y * direction.y);
		direction.y /= sqrtf(direction.x * direction.x + direction.y * direction.y);
	}

	newPosition.x = position.x + direction.x * vel * deltaT;
	newPosition.y = position.y + direction.y * vel * deltaT;

	if (newPosition.x >= 0 && newPosition.x <= BASE_WIDTH - texture.width)
		position.x = newPosition.x;

	if (newPosition.y >= 0 && newPosition.y <= BASE_HEIGHT - texture.height)
		position.y = newPosition.y;

	if (isBigLaserActive)
	{
		bigLaser.position.x = position.x + (texture.width / 2.0f) - (BigLaser::texture.width / 2.0f);
		bigLaser.position.y = position.y - (BigLaser::texture.height);
	}
	else if (IsKeyPressed(KeyBinds.KeySHOOT) && !cooldownTimerLaser.isRunning)
	{
		const float laserOffset = 30.0f * scale;
		const float laserCenterX = position.x + (texture.width / 2.0f) - (Laser::texture.width / 2.0f);
		const float laserY = position.y - laserOffset;

		lasers.emplace_back(laserCenterX, laserY, laserTimeToLive);

		if (tripleLaser)
		{
			lasers.emplace_back(laserCenterX - laserOffset, laserY);
			lasers.emplace_back(laserCenterX + laserOffset, laserY);
		}
		cooldownTimerLaser.active();
	}

	direction.x = 0;
	direction.y = 0;
}

Rectangle Player::getBounds()
{
    return { position.x, position.y, static_cast<float>(texture.width), static_cast<float>(texture.height) };
}
