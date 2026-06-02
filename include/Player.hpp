#pragma once
#include "Entity.hpp"
#include "Map.hpp"
#include <queue>
#include <SFML/Graphics.hpp>

class Player : public Entity
{
public:
    Player()
    {
        data.setMaxHp(100), data.setMaxMp(100);
        data.init();

        manager.setColor(sf::Color::Blue);
    }

    bool handleInput(int dx, int dy, World& world);
    void updateAction(World& world) override;
    void dead(World& world, sf::RenderWindow& window) override;
private:
};