#pragma once

#include <typeindex>
#include <memory>
#include <unordered_map>

#include "EntityContainer.hpp"
#include "../IGameMode/IGameMode.hpp"

class EntityManager
{
    std::unordered_map<std::type_index, std::unique_ptr<IEntityContainer>> containers;
    GameContext& ctx;

public:
    EntityManager(GameContext& ctx) : ctx(ctx) {}

    template <typename T>
    EntityContainer<T>* getContainer()
    {
        auto i = std::type_index(typeid(T));
        if (containers.find(i) == containers.end())
        {
            containers[i] = std::make_unique<EntityContainer<T>>(ctx);
        }
        return static_cast<EntityContainer<T>*>(containers[i].get());
    }

    void Update(float dt)
    {
        for (auto& [type, container] : containers)
            if (container) container->UpdateAll(dt);
    }

    void Draw()
    {
        for (auto& [type, container] : containers)
            if (container) container->DrawAll();
    }

    void ClearAllContainers()
    {
        for (auto& [type, container] : containers)
            if (container) container.reset();
    }
};
