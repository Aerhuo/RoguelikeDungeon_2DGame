#include "World.hpp"

World::World(int width, int height) : map(width, height)
{   
    map.generate();
    sf::Vector2i pos = map.getRandomFloorTile();

    // 生成玩家
    while (map.getEntityAt(pos) != nullptr) pos = map.getRandomFloorTile();
    player.manager.setPosition(pos);
    player.data.init();
    player.spawn(map);
    player.fogManager.init(width, height);

    player.fogManager.update(this);
}

void World::init()
{
    dist.resize(map.getWidth(), std::vector<int>(map.getHeight()));
}