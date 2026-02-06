#pragma once

#include <vector>
#include "../specs.hpp"
#include "../meteor.hpp"
#include "../laser.hpp"
#include "../player.hpp"
#include "../explosion.hpp"
#include "../heart.hpp"
#include "../modifier.hpp"

class CollisionManager
{
public:
    ~CollisionManager() = default;
    virtual void ResolveAll(
        Player& player,
        std::vector<Meteor>& meteors,
        std::vector<Laser>& lasers,
        std::vector<Modifier>& modifiers,
        std::vector<Explosion>& explosions,
        int& score,
        std::shared_ptr<GameState>& status,
        std::vector<Heart>& hearts,
        int maxHearts
    );
private:
    void HandleLaserMeteors(Player& player, std::vector<Laser>& lasers, std::vector<Meteor>& meteors, std::vector<Explosion>& explosions, int& score);
    void HandlePlayerMeteors(Player& player, std::vector<Meteor>& meteors, std::shared_ptr<GameState>& status, std::vector<Heart>& hearts);
    void HandlePlayerModifiers(Player& player, std::vector<Modifier>& modifiers, std::shared_ptr<GameState>& status, std::vector<Heart>& hearts, int maxHearts);
};
