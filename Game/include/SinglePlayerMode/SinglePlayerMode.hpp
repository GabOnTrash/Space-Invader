#pragma once

#include "../specs.hpp"

#include "../stars.hpp"
#include "../player.hpp"
#include "../meteor.hpp"
#include "../explosion.hpp"
#include "../modifier.hpp"
#include "../heart.hpp"
#include "../timer.hpp"
#include "../MenuHandle/MenuHandle.hpp"
#include "../../../Network/src/Client.cpp"
#include "CollisionManager/CollisionManager.hpp"

#include "IGameMode/IGameMode.hpp"

class SinglePlayerMode : public IGameMode
{
public:
    SinglePlayerMode(GameContext& ctx);
	~SinglePlayerMode();

    void Update(float dt);
    void Resume() override;

    CollisionManager cManager;

    int GameScore = 0;
    float dt = 0.0f;
    int modifierType = 0;

    std::vector<Laser> lasers;
    std::vector<Heart> heartsArray;
    std::vector<Meteor> meteors;
    std::vector<Explosion> explosions;
    std::vector<Modifier> modifiers;

private:
    void Start();
    void UpdateGamePlay();
    void DrawGamePlay();
    void UpdateElements();
    void UpdateTimers();
    void SpawnModifier();
    void ClearEffects();

    Timer<> meteorTimer;
    Timer<> modifierTimer;

    int nMaxHearts = 0;
    int DiffPerModifiers = 0;
};
    // Client client;

    /*static inline Texture2D instagram = {};
    static inline Texture2D github = {};*/
    // delegate to menu Timer<> timerDelayResume;

    /*
     // delegate to the menu
    void DrawLinks() const;

    static void InitIcons();
*/
