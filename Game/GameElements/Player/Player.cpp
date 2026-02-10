#include "Player.hpp"
#include "../Meteor/Meteor.hpp"

Player::Player(GameContext& ctx)
    : cooldownTimerLaser(coolDown),
    ctx(ctx),
    cooldownTimerDash(1000),
    tripleLaserTimer(5000, nullptr),
    bigLaserTimer(6000, nullptr),
    reducedVelTimer(5000, nullptr)
{
    texture = AssetsManager::GetTexture("player");

    static bool byteMaskLoaded = false;
    if (!byteMaskLoaded)
    {
        byteMask.loadFromImage(PATH_BYTEMASK_PLAYER);
        byteMaskLoaded = true;
    }
}
Player::~Player()
{
}

void Player::Reset()
{
	position.x = ((ctx.renderer.BASE_WIDTH / 2.0f) - (texture.width / 2.0f));
	position.y = (ctx.renderer.BASE_HEIGHT / 2.0f);
	vel = 500;

	cooldownTimerLaser.deactive();
    reducedVelTimer.deactive();
    bigLaserTimer.deactive();
}

void Player::Draw()
{
    if (bigLaserTimer.isRunning)
        bigLaser.Draw();
    
    DrawTexture(texture, static_cast<int>(position.x), static_cast<int>(position.y), WHITE);
}
void Player::Update(float dt)
{
	Movement(dt);
    Meteor::playerPos = position;
	cooldownTimerLaser.update();
    cooldownTimerDash.update();
    tripleLaserTimer.update();
    reducedVelTimer.update();
    bigLaserTimer.update();
}
void Player::UpdateDash(float dt)
{
    dashTimer += dt;
	float t = dashTimer / dashDuration;
	if (t >= 1.0f)
	{
		isDashing = false;
        cooldownTimerDash.active();
		return;
	}
	
	float curve = sinf(t * PI);
    float dashSpeed = vel * dashMultiplier * curve;

    position.x += dashDir.x * dashSpeed * dt;
    position.y += dashDir.y * dashSpeed * dt;

    if (position.x <= 0) position.x = 0;
    if (position.y <= 0) position.y = 0;
    if (position.x >= ctx.renderer.BASE_WIDTH - texture.width) position.x = ctx.renderer.BASE_WIDTH - texture.width;
    if (position.y >= ctx.renderer.BASE_HEIGHT - texture.height) position.y = ctx.renderer.BASE_HEIGHT - texture.height;
}

void Player::StartDash(Vector2 dir)
{
    if (isDashing)
        return;

    isDashing = true;
    dashTimer = 0.0f;
    dashDir = dir;
}

void Player::HandleInput()
{
    if (IsKeyDown(ctx.keyBindings.KeyUP))
        direction.y -= 1.f;
    if (IsKeyDown(ctx.keyBindings.KeyDOWN))
        direction.y += 1.f;
    if (IsKeyDown(ctx.keyBindings.KeyLEFT))
        direction.x -= 1.f;
    if (IsKeyDown(ctx.keyBindings.KeyRIGHT))
        direction.x += 1.f;

    if (sqrtf(direction.x * direction.x + direction.y * direction.y) != 0)
    {
        direction.x /= sqrtf(direction.x * direction.x + direction.y * direction.y);
        direction.y /= sqrtf(direction.x * direction.x + direction.y * direction.y);
    }

    if ((IsKeyPressed(ctx.keyBindings.KeyDASH) && Vector2Length(direction) != 0) && !cooldownTimerDash.isRunning)
        StartDash(direction);
}

void Player::Movement(float dt)
{
    if (isDashing)
        UpdateDash(dt);

    vel = reducedVelTimer.isRunning ? 250 : 500;

    HandleInput();

    newPosition.x = position.x + direction.x * vel * dt;
    newPosition.y = position.y + direction.y * vel * dt;

    if (newPosition.x >= 0 && newPosition.x <= ctx.renderer.BASE_WIDTH - texture.width)
        position.x = newPosition.x;

    if (newPosition.y >= 0 && newPosition.y <= ctx.renderer.BASE_HEIGHT - texture.height)
        position.y = newPosition.y;

    if (bigLaserTimer.isRunning)
    {
        bigLaser.position.x = position.x + (texture.width / 2.0f) - (BigLaser::texture.width / 2.0f);
        bigLaser.position.y = position.y - (BigLaser::texture.height);
    }
    else if ((IsKeyPressed(ctx.keyBindings.KeySHOOT) || IsKeyDown(ctx.keyBindings.KeySHOOT)) && !cooldownTimerLaser.isRunning)
        wantToGenerateLaser = true;

    direction = { 0.f, 0.f };
}
void Player::generateLaser(std::vector<Laser>& lasers)
{
    const float laserCenterX = position.x + (texture.width / 2.0f) - (Laser::texture.width / 2.0f);
    const float laserY = position.y - Laser::texture.height;

    lasers.emplace_back(Vector2{ laserCenterX, laserY }, laserTimeToLive);

    if (tripleLaserTimer.isRunning)
    {
        lasers.emplace_back(Vector2{ laserCenterX - Laser::texture.width * 2, laserY }, laserTimeToLive);
        lasers.emplace_back(Vector2{ laserCenterX + Laser::texture.width * 2, laserY }, laserTimeToLive);
    }
    cooldownTimerLaser.active();
}

void Player::setLaserTimeToLive(int timeInMs)
{
    laserTimeToLive = timeInMs;
}
void Player::setDashTimer(int timeInMs)
{
    dashTimer = timeInMs;
}

Rectangle Player::getBounds()
{
    return { position.x, position.y, static_cast<float>(texture.width), static_cast<float>(texture.height) };
}

void Player::ActivateBigLaser()
{
    bigLaserTimer.active();
}

void Player::ActivateTripleLaser()
{
    tripleLaserTimer.active();
}

void Player::ActivateReducedVelocity()
{
    reducedVelTimer.active();
}
