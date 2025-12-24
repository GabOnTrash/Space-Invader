#include "player.hpp"

Player::Player() : cooldownTimerLaser(coolDown), cooldownTimerDash(1000)
{
	Reset();
}
Player::~Player()
{
}

void Player::Init()
{
	if (ViewPort::loadedResources)
	{
		texture = AssetsManager::GetTexture("player");
        byteMask.loadFromImage(PATH_BYTEMASK_PLAYER);
	}
}

void Player::Reset()
{
	lasers.clear();
	lasers.reserve(10);

	position.x = ((ViewPort::BASE_WIDTH / 2.0f) - (texture.width / 2.0f));
	position.y = (ViewPort::BASE_HEIGHT / 2.0f);
	vel = normalVelocity;

	cooldownTimerLaser.deactive();

	tripleLaser = false;
	reducedVel = false;
	isBigLaserActive = false;
	clearLaser();
}

void Player::Draw()
{
    DrawTexture(texture, static_cast<int>(position.x), static_cast<int>(position.y), WHITE);
}

void Player::Update(float deltaT)
{
	Movement(deltaT);
	cooldownTimerLaser.update();
    cooldownTimerDash.update();
}

void Player::clearLaser()
{
	lasers.clear();
}

void Player::UpdateDash(float deltaT)
{
    dashTimer += deltaT;
	float t = dashTimer / dashDuration;
	if (t >= 1.0f)
	{
		isDashing = false;
        cooldownTimerDash.active();
		return;
	}
	
	float curve = sinf(t * PI);
    float dashSpeed = vel * dashMultiplier * curve;

    position.x += dashDir.x * dashSpeed * deltaT;
    position.y += dashDir.y * dashSpeed * deltaT;
}

void Player::StartDash(Vector2 dir)
{
    isDashing = true;
    dashTimer = 0.0f;
    dashDir = dir;
}

void Player::HandleInput()
{
    if (IsKeyDown(KeyBinds.KeyUP))
        direction.y -= 1;
    if (IsKeyDown(KeyBinds.KeyDOWN))
        direction.y += 1;
    if (IsKeyDown(KeyBinds.KeyLEFT))
        direction.x -= 1;
    if (IsKeyDown(KeyBinds.KeyRIGHT))
        direction.x += 1;

    if (sqrtf(direction.x * direction.x + direction.y * direction.y) != 0)
    {
        direction.x /= sqrtf(direction.x * direction.x + direction.y * direction.y);
        direction.y /= sqrtf(direction.x * direction.x + direction.y * direction.y);
    }

    if (!IsKeyPressed(KeyBinds.KeyDASH) || Vector2Length(direction) == 0 || cooldownTimerDash.isRunning)
        return;

    StartDash(direction);
}

void Player::Movement(float deltaT)
{
    if (isDashing)
        UpdateDash(deltaT);

    vel = reducedVel ? reducedVelocity : normalVelocity;

    HandleInput();

    newPosition.x = position.x + direction.x * vel * deltaT;
    newPosition.y = position.y + direction.y * vel * deltaT;

    if (newPosition.x >= 0 && newPosition.x <= ViewPort::BASE_WIDTH - texture.width)
        position.x = newPosition.x;

    if (newPosition.y >= 0 && newPosition.y <= ViewPort::BASE_HEIGHT - texture.height)
        position.y = newPosition.y;

    if (isBigLaserActive)
    {
        bigLaser.position.x = position.x + (texture.width / 2.0f) - (BigLaser::texture.width / 2.0f);
        bigLaser.position.y = position.y - (BigLaser::texture.height);
    }
    else if (IsKeyPressed(KeyBinds.KeySHOOT) && !cooldownTimerLaser.isRunning)
        generateLaser();

    direction = { 0, 0 };
}
void Player::generateLaser()
{
    const float laserOffset = 30.0f;
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
Rectangle Player::getBounds()
{
    return { position.x, position.y, static_cast<float>(texture.width), static_cast<float>(texture.height) };
}
