#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Player;
class World;

enum class FogState
{
    UNSEEN,
    EXPLORED,
    VISIBLE
};

class FogManager
{
public:
    FogManager(Player* player);

    void init(int width, int height);
    void update(World* world);

    // get器
    FogState getStateAt(sf::Vector2i pos) const
    {
        if (pos.x < 0 || pos.x >= width || pos.y < 0 || pos.y >= height) return FogState::UNSEEN;
        return grids[pos.x][pos.y];
    }
    // set器

private:
    std::vector<sf::Vector2i> visibleCells;
    std::vector<std::vector<FogState>> grids;
    Player* owner;
    int width, height;
};