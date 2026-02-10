#include "Meteor.hpp"

Meteor::Meteor(Vector2 pos)
{
	damage = AssetsManager::GetSound("meteor_damage");

	arraymeteors[0] = AssetsManager::GetTexture("meteor");
	arraymeteors[1] = AssetsManager::GetTexture("meteor_damaged");

	static bool byteMaskLoaded = false;
	if (!byteMaskLoaded)
	{
		byteMask.loadFromImage(PATH_BYTEMASK_ASTEROID);
		byteMaskLoaded = true;
	}
    texture = arraymeteors;

	velocity = { GetRN<float>(250.f, 300.f), GetRN<float>(250.f, 300.f) };

	position = pos;

	direction.x = playerPos.x - position.x;
	direction.y = playerPos.y - position.y;

	float len = sqrt(direction.x * direction.x +
					 direction.y * direction.y);

	if (len != 0)
	{
		direction.x /= len;
		direction.y /= len;
	}
}
Meteor::~Meteor()
{
}
Rectangle Meteor::getBounds()
{
	return { position.x, position.y, static_cast<float>(texture->width), static_cast<float>(texture->height) };
}

void Meteor::Draw()
{
	DrawTexture(*texture, static_cast<int>(position.x), static_cast<int>(position.y), WHITE);
}
void Meteor::Update(float dt)
{
    position.x += velocity.x * direction.x * dt;
    position.y += velocity.y * direction.y * dt;
}

bool Meteor::ShouldDie(float checkValue)
{
	return shouldDie || (getBounds().y > checkValue + texture->height);
}

void Meteor::NextState()
{
	SetSoundVolume(damage, volume);
	PlaySound(damage);

	switch (meteorState)
	{
	case MeteorState::INTEGRITY:
		meteorState = MeteorState::DAMAGED;
		texture = &arraymeteors[1];
		break;
	case MeteorState::DAMAGED:
		shouldDie = true;
		break;
	}
}
