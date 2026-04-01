#pragma once

#include <vector>

class IEntityContainer
{
protected:
    GameContext& ctx;

public:
    IEntityContainer(GameContext& ctx) : ctx(ctx) {}

    virtual ~IEntityContainer() = default;
    virtual void UpdateAll(float dt) = 0;
    virtual void DrawAll() = 0;
    virtual void Clear() = 0;
};

template <typename T>
class EntityContainer : public IEntityContainer
{
    std::vector<T> elements;

public:
    EntityContainer(GameContext& ctx) : IEntityContainer(ctx) {}

    T& add(T&& element)
    {
        elements.push_back(std::move(element));
        return elements.back();
    }

    template <typename ...Args>
    T& emplace(Args&&... args)
    {
        return elements.emplace_back(std::forward<Args>(args)...);
    }

    void UpdateAll(float dt) override
    {
        for (size_t i = 0; i < elements.size();)
        {
            elements[i].Update(dt);
            if (CheckLifeSpan(elements[i]))
                i++;
        }
    }

    void DrawAll() override
    {
        for (auto& e : elements)
            e.Draw();
    }

    void Clear() override
    {
        elements.clear();
    }

    bool CheckLifeSpan(T& elem)
    {
        if (elem.ShouldDie(ctx.renderer.BASE_HEIGHT))
        {
            elem = std::move(elements.back());
            elements.pop_back();
            return false;
        }
        return true;
    }

    std::vector<T>& getElements() { return elements; }
};
