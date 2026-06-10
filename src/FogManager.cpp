#include "FogManager.hpp"
#include "Player.hpp"

FogManager::FogManager(Player* player) : owner(player)
{
}

void FogManager::init(int width, int height)
{
    this->width = width, this->height = height;
    grids.resize(width, std::vector<FogState>(height));
}

void FogManager::update(World* world)
{
    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            if (grids[x][y] == FogState::VISIBLE) grids[x][y] = FogState::EXPLORED;
        }
    }

    auto filter = [&](sf::Vector2i pos) -> void
    {
        if (pos.x < 0 || pos.x >= width || pos.y < 0 || pos.y >= height) return;
        grids[pos.x][pos.y] = FogState::VISIBLE;
    };

    FOVEngine::calculateFOV(owner->manager.getPos(), owner->manager.getDir(), owner->data.getFOV(), filter, world);
}