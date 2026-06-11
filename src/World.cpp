#include "World.hpp"
#include "Map.hpp"
#include "EnemyFactory.hpp"

World::World()
{   
    player.data.init();
    generateNextFloor();
}

void World::init()
{
    dist.resize(map.getWidth(), std::vector<int>(map.getHeight()));
}

void World::generateNextFloor()
{
    currentFloor++;

    enemies.clear();

    int width = MapHeight, height = MapHeight;

    // 随机地图预设
    int choice = rand() % 4;
    int walkPercent = 57;
    int autoMataTimes = 3;

    switch (choice)
    {
        case 0:
            walkPercent = 57; 
            autoMataTimes = 3; 
            break;
        case 1:
            walkPercent = 42; 
            autoMataTimes = 7; 
            break;
        case 2:
            walkPercent = 68; 
            autoMataTimes = 3; 
            break;
        case 3:
            walkPercent = 50; 
            autoMataTimes = 1; 
            break;
    }

    map.init(width, height, walkPercent, autoMataTimes);
    map.generate();
    
    sf::Vector2i pos = map.getRandomFloorTile();

    // 生成玩家
    while (map.getEntityAt(pos) != nullptr) pos = map.getRandomFloorTile();
    player.manager.setPosition(pos);
    player.spawn(map);
    player.fogManager.init(width, height);

    player.fogManager.update(*this);

    while (map.getEntityAt(pos) != nullptr) pos = map.getRandomFloorTile();
    map.setTerrainGridType(pos, TileType::STAIRS_DOWN);

    // 生成怪物
    EnemyFactory::spawnEnemies(*this, EnemyType::SLIME, 5);
}