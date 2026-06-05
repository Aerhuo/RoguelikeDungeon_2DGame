#include "EnemyFactory.hpp"
#include "Enemy.hpp"

void EnemyFactory::spawnEnemies(World& world, EnemyType type, int count)
{
    for (int i = 0; i < count; ++i)
    {
        sf::Vector2i pos = world.map.getRandomFloorTile();
        while (world.map.getEntityAt(pos) != nullptr)
        {
            pos = world.map.getRandomFloorTile();
        }

        std::unique_ptr<Enemy> newEnemy;
        switch (type)
        {
            case EnemyType::SLIME:
            {
                newEnemy = std::make_unique<Slime>();
                break;
            }
            default: continue; //跳出循环
        }

        if (newEnemy == nullptr) break;

        newEnemy->manager.setPosition(pos);
        newEnemy->data.init();
        newEnemy->spawn(world.map);

        world.enemies.push_back(std::move(newEnemy));
    }
}