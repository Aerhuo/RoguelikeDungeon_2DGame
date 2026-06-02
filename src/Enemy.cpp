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

void Slime::updateAction(World& world)
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

    if (cur[0] != 0 || cur[1] != 0)
    {
        bump(cur[0], cur[1], world);
    }
}

void Slime::dead(World& world, sf::RenderWindow& window)
{

}