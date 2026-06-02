#include "Enemy.hpp"
#include "World.hpp"

void Slime::updateAction(World& world)
{
    std::vector<std::vector<int>> dirs = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };

    std::vector<int> cur = {-1, 0};
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

    Event ev;
    ev.actor = this;
    ev.type = EventType::MOVE;
    ev.dx = cur[0];
    ev.dy = cur[1];
    
    world.eventQueue.push(ev);
}

void Slime::dead(World& world, sf::RenderWindow& window)
{

}