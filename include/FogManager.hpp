#pragma once
#include <vector>
#include "FOVEngine.hpp"
#include "Map.hpp"
#include <SFML/Graphics.hpp>

class Player;

enum class FogState
{
    UNSEEN,
    EXPLORED,
    VISIBLE
};

class FogManager
{
public:
    FogManager(int width, int height, Player* player);

    void update(World& world);

    // get器
    FogState getStateAt(sf::Vector2i pos)
    {
        if (pos.x < 0 || pos.x >= width || pos.y < 0 || pos.y >= height) return FogState::UNSEEN;
        return grids[pos.x][pos.y];
    }
    // set器

private:
    std::vector<std::vector<FogState>> grids;
    Player* player;
    int width, height;
};