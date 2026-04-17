#pragma once

#include "../IGameMode/IGameMode.hpp"

#include "../Meteor/Meteor.hpp"
#include "../Explosion/Explosion.hpp"
#include "../Modifier/Modifier.hpp"
#include "../Heart/Heart.hpp"
#include "../util/timer.hpp"

#include "../EntitiesManaging/EntityManager.hpp"

class SinglePlayerMode : public IGameMode
{
public:
    SinglePlayerMode(GameContext& ctx, MenuHandle& handle);
	~SinglePlayerMode();

    void Init() override;
    void Update(float dt) override;
    void Draw() override;
    void OnExit() override;

    int GameScore = 0;
    int lastScore = 0;
    int modifierType = 0;

private:
    void ClearEffects();
    void ShowCountDown();
    void CreateMeteor();
    void CreateModifier();

    EntityManager entityManager;

    Timer<> meteorTimer;
    Timer<> modifierTimer;
    Timer<> timerDelayResume;
};
