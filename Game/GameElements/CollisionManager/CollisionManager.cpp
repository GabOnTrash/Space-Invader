#include "CollisionManager.hpp"

void CollisionManager::ResolveAll(Player& player,
        std::vector<Meteor>& meteors,
        std::vector<Laser>& lasers,
        std::vector<Modifier>& modifiers,
        std::vector<Explosion>& explosions,
        int& score,
        std::shared_ptr<GameState>& status,
        std::vector<Heart>& hearts,
        int maxHearts)
{
    HandleLaserMeteors(player, lasers, meteors, explosions, score);
    HandlePlayerMeteors(player, meteors, status, hearts);
    HandlePlayerModifiers(player, modifiers, status, hearts, maxHearts);
}

void CollisionManager::HandleLaserMeteors(Player& player, std::vector<Laser>& lasers, std::vector<Meteor>& meteors,
    std::vector<Explosion>& explosions, int& score)
{
    for (size_t i = 0; i < lasers.size();)
    {
        bool laserErased = false;
        for (size_t j = 0; j < meteors.size();)
        {
            Rectangle rLaser = lasers[i].getBounds();
            Rectangle rAst = meteors[j].getBounds();

            if (CheckCollisionRecs(rLaser, rAst))
            {
                if (Laser::byteMask.checkPixelCollision(Meteor::byteMask, lasers[i].position, meteors[j].position, GetCollisionRec(rLaser, rAst)))
                {
                    lasers.erase(lasers.begin() + i);
                    laserErased = true;

                    meteors[j].NextState();

                    if (!meteors[j].shouldDie)
                        break;

                    explosions.emplace_back(meteors[j].getBounds());
                    meteors[j] = meteors.back();
                    meteors.pop_back();
                    score++;
                }
            }
            j++;
        }
        if (!laserErased)
            i++;
    }

    if (player.activeBigLaser())
    {
        for (size_t i = 0; i < meteors.size();)
        {
            Rectangle rLaser = player.getBigLaser().getBounds();
            Rectangle rAst = meteors[i].getBounds();

            if (CheckCollisionRecs(rLaser, rAst))
            {
                if (BigLaser::byteMask.checkPixelCollision(Meteor::byteMask, player.getBigLaser().position, meteors[i].position, GetCollisionRec(rLaser, rAst)))
                {
                    meteors[i].NextState();

                    if (!meteors[i].shouldDie)
                        break;

                    explosions.emplace_back(meteors[i].getBounds());
                    meteors[i] = meteors.back();
                    meteors.pop_back();
                    score++;
                }
            }
            i++;
        }
    }
}

void CollisionManager::HandlePlayerMeteors(Player& player, std::vector<Meteor>& meteors, std::shared_ptr<GameState>& status,
    std::vector<Heart>& hearts)
{
    for (size_t i = 0; i < meteors.size();)
    {
        Rectangle rPlayer = player.getBounds();
        Rectangle rAsteroid = meteors[i].getBounds();

        if (CheckCollisionRecs(rPlayer, rAsteroid))
        {
            if (Player::byteMask.checkPixelCollision(Meteor::byteMask, player.position, meteors[i].position, GetCollisionRec(rPlayer, rAsteroid)))
            {
                if (hearts.size() == 0)
                {
                    *status = GameState::KILLED;
                    return;
                }

                hearts.pop_back();
                meteors[i] = meteors.back();
                meteors.pop_back();
                continue;
            }
        }
        i++;
    }
}

void CollisionManager::HandlePlayerModifiers(Player& player, std::vector<Modifier>& modifiers, std::shared_ptr<GameState>& status,
    std::vector<Heart>& hearts, int maxHearts)
{
    for (size_t i = 0; i < modifiers.size();)
    {
        Rectangle rPlayer = player.getBounds();
        Rectangle rPot = modifiers[i].getBounds();

        if (CheckCollisionRecs(rPlayer, rPot))
        {
            if (Player::byteMask.checkPixelCollision(Modifier::byteMask, player.position, modifiers[i].position, GetCollisionRec(rPlayer, rPot)))
            {
                switch (modifiers[i].modType)
                {
                case ModifierType::SLOWER:
                    player.ActivateReducedVelocity();
                    break;

                case ModifierType::LOSEHEART:
                    if (!hearts.empty()) hearts.pop_back();
                    else *status = GameState::KILLED;
                    break;

                case ModifierType::TRIPLELASER:
                    player.ActivateTripleLaser();
                    break;

                case ModifierType::GAINHEART:
                    if (hearts.size() < maxHearts) hearts.emplace_back();
                    break;

                case ModifierType::BIGLASER:
                    player.ActivateBigLaser();
                    break;
                }

                modifiers[i] = modifiers.back();
                modifiers.pop_back();
                continue;
            }
        }
        i++;
    }
}