#include "meteor.hpp"

Meteor::Meteor()
{
    texture = arraymeteors;

	velX = GetRN<int>(250, 300);
	velY = GetRN<int>(250, 300);

	position.x = GetRN<float>(-ViewPort::BASE_WIDTH, ViewPort::BASE_WIDTH);
    position.y = GetRN<float>(-texture->height * 2.0f, -texture->height * 1.5f);

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

void Meteor::Init()
{
	if (ViewPort::loadedResources)
	{
		arraymeteors[0] = AssetsManager::GetTexture("meteor");
        arraymeteors[1] = AssetsManager::GetTexture("meteor_damaged");
        byteMask.loadFromImage(PATH_BYTEMASK_ASTEROID);

		audio = AssetsManager::GetSound("meteor_damage");
	}
}

void Meteor::Draw()
{
	DrawTexture(*texture, static_cast<int>(position.x), static_cast<int>(position.y), WHITE);
}
void Meteor::Update(float deltaT)
{
    position.x += velX * direction.x * deltaT;
    position.y += velY * direction.y * deltaT;
}

void Meteor::NextState()
{
	SetSoundVolume(audio, volume);
	PlaySound(audio);
	AsteroidState = static_cast<MeteorState>(AsteroidState + 1);

	if (AsteroidState == DAMAGED) 
		texture = &arraymeteors[1];
}
MeteorState Meteor::getState()
{
	return static_cast<MeteorState>(AsteroidState);
}
