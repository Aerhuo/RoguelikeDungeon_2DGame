#include "FogManager.hpp"
#include "Player.hpp"
#include "World.hpp"
#include "FOVEngine.hpp"

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
    for (const auto& pos : visibleCells)
    {
        grids[pos.x][pos.y] = FogState::EXPLORED;
    }
    visibleCells.clear();

    auto filter = [&](sf::Vector2i pos) -> void
    {
        if (grids[pos.x][pos.y] != FogState::VISIBLE)
        {
            grids[pos.x][pos.y] = FogState::VISIBLE;
            visibleCells.push_back(pos);
        }
    };

    FOVEngine::calculateFOV(owner->manager.getPos(), owner->manager.getDir(), owner->data.getFOV(), filter, world);
}