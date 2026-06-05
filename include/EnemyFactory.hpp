#pragma once
#include "World.hpp"
#include <memory>

enum class EnemyType
{
    SLIME
};

class EnemyFactory
{
public:
    static void spawnEnemies(World& world, EnemyType type, int count);
};