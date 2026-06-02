#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.hpp"
#include <queue>
#include <vector>

class Slime : public Entity
{
public:
    Slime()
    {
        data.setMaxHp(10), data.setMaxMp(10);
        data.init();

        manager.setColor(sf::Color::Red);
    }

    void updateAction(World& world) override;
    void dead(World& world, sf::RenderWindow& window) override;
private:
};

