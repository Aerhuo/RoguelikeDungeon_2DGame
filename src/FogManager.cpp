#include "FogManager.hpp"

FogManager::FogManager(int width, int height, Player* player) : player(player)
{
    grids.resize(width, std::vector<FogState>(height));
}

void FogManager::update(World& world)
{

}