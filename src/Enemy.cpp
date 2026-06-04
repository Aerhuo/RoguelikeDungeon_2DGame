#include "Enemy.hpp"
#include "World.hpp"

Slime::Slime()
{
    data.setMaxHp(10.0), data.setMaxMp(10.0);
    data.setDamage(10.0);
    data.setTeam(2);
    data.init();

    manager.setColor(sf::Color::Red);
}

sf::Vector2i findPlayerRoad(const EntityManager& manager, const World& world)
{
    std::vector<std::vector<int>> dirs = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };

    std::vector<int> cur = {0, 0};
    int x = manager.getPos().x, y = manager.getPos().y;
    for (auto& dir : dirs)
    {
        if (world.dist[x + dir[0]][y + dir[1]] < world.dist[x + cur[0]][y + cur[1]])
        {
            cur = dir;
        }

        if (world.dist[x + dir[0]][y + dir[1]] == world.dist[x + cur[0]][y + cur[1]] && rand() % 100 < 50)
        {
            cur = dir;
        }
    }

    return sf::Vector2i(cur[0], cur[1]);
}

sf::Vector2i Enemy::findWanderRoad(World& world)
{
    std::vector<sf::Vector2i> dirs = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };

    int totalWeight = 0;
    struct DirWeight
    {
        sf::Vector2i dir;
        int weight;
    };

    std::vector<DirWeight> choices;

    for (auto& dir : dirs)
    {
        // 目标方向存在墙壁则直接跳过该方向
        if (world.map.getTerrainGridType(dir + manager.getPos()) == 0) continue;

        int weight;
        if (dir == lastDir)
        {
            weight = 60;
        }
        else if (dir == -lastDir)
        {
            weight = 10;
        }
        else
        {
            weight = 30;
        }

        totalWeight += weight;
        choices.push_back({dir, weight});
    }

    // 未找到可用道路，原地待命
    if (totalWeight == 0)
    {
        return sf::Vector2i(0, 0);
    }

    int randomVal = rand() % totalWeight;
    int curWeight = 0;

    for (auto& choice : choices)
    {
        curWeight += choice.weight;

        if (curWeight > randomVal)
        {
            lastDir = choice.dir;
            return choice.dir;
        }
    }

    return sf::Vector2i(0, 0);
}

void Slime::updateAction(World& world)
{
    auto fliter = [&](Entity* entity) -> bool
    {
        if (entity == nullptr) return false;
        return entity->data.getTeam() != data.getTeam();
    };

    std::vector<Entity*> entities = world.map.getEntitiesInRadius(manager.getPos(), 5, fliter);
    if (!entities.empty())
    {
        state = EnemyState::CHASE;
    }
    else
    {
        state = EnemyState::WANDER;
    }

    sf::Vector2i dPos;
    if (state == EnemyState::CHASE)
    {
        dPos = findPlayerRoad(manager, world);
    }
    else if (state == EnemyState::WANDER)
    {
        dPos = findWanderRoad(world);
    }
    else if (state == EnemyState::ESCAPE)
    {

    }
    else
    {
        // 其它状态……
    }

    if (dPos.x != 0 || dPos.y != 0)
    {
        bump(dPos.x, dPos.y, world);
    }
}

void Slime::dead(World& world, sf::RenderWindow& window)
{

}