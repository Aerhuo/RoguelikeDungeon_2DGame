#pragma once
#include "Entity.hpp"
#include <queue>
#include <SFML/Graphics.hpp>
#include "FogManager.hpp"

class Player : public Entity
{
public:
    Player();

    FogManager fogManager;

    bool handleInput(int dx, int dy, World& world);
    void updateAction(World& world) override;
    void dead(World& world, sf::RenderWindow& window) override;
private:
};