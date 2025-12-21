#include "meteor.hpp"

Meteor::Meteor()
{
    texture = arraymeteors;

	velX = GetRN<int>(250, 300);
	velY = GetRN<int>(250, 300);
	direction =	GetRN<float>(-1, 1);
	position.x = GetRN<float>(50, ViewPort::BASE_WIDTH - 50);
    position.y = GetRN<float>(-texture->height * 2.0f, -texture->height * 1.5f);

	if (direction == 0)
		direction = 1;

	direction *= 0.7f;
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
	DrawTexture(*texture, position.x, position.y, WHITE);
}
void Meteor::Update(float deltaT)
{
    position.x += velX * direction * deltaT;
    position.y += velY * deltaT;
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
