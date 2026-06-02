#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.hpp"
#include <queue>
#include <vector>

class Slime : public Entity
{
public:
    Slime();

    void updateAction(World& world) override;
    void dead(World& world, sf::RenderWindow& window) override;
private:
};

