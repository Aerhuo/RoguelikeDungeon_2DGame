#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.hpp"
#include <queue>
#include <vector>

enum class EnemyState
{
    WANDER,
    CHASE,
    ESCAPE
};

class Enemy : public Entity
{
protected:
    sf::Vector2i findWanderRoad(World& world);
};

class Slime : public Enemy
{
public:
    Slime();

    void updateAction(World& world) override;
    void dead(World& world, sf::RenderWindow& window) override;
private:
    EnemyState state = EnemyState::WANDER;
};

