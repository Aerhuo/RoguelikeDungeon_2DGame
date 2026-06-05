#include "FOVEngine.hpp"
#include "Map.hpp"
#include "World.hpp"

void FOVEngine::calculateFOV(sf::Vector2i center, int radius, const std::function<void(sf::Vector2i)>& onVisable, World& world)
{
    int startX = std::max(0, center.x - radius);
    int startY = std::max(0, center.y - radius);
    int endX = std::min(MapWidth - 1, center.x + radius);
    int endY = std::min(MapHeight - 1, center.y + radius);

    int radius2 = radius * radius;
    for (int x = startX; x <= endX; ++x)
    {
        castRay(center, sf::Vector2i(x, startY), radius2, onVisable, world);
        castRay(center, sf::Vector2i(x, endY), radius2, onVisable, world);
    }
    for (int y = startY; y <= endY; ++y)
    {
        castRay(center, sf::Vector2i(startX, y), radius2, onVisable, world);
        castRay(center, sf::Vector2i(endX, y), radius2, onVisable, world);
    }
}

void FOVEngine::castRay(sf::Vector2i start, sf::Vector2i end, int radius2, const std::function<void(sf::Vector2i)>& onVisalbe, World& world)
{
    int x0 = start.x, y0 = start.y;
    int x1 = end.x, y1 = end.y;

    int dx = abs(x0 - x1), dy = (y0 - y1);
    int sx = (x0 < x1 ? 1 : -1);
    int sy = (y0 < y1 ? 1 : -1);

    int err = dx - dy;
    while (true)
    {
        if (x0 < 0 || x0 >= MapWidth || y0 < 0 || y0 >= MapHeight) break;

        int dist = (x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1);
        if (dist <= radius2)
        {
            onVisalbe(sf::Vector2i(x0, y0));
        }

        if (!world.map.canMove(sf::Vector2i(x0, y0))) break;
        if (sf::Vector2i(x0, y0) == end) break;

        int e2 = err * 2;
        if (e2 > -dy)
        {
            err = -dy;
            x0 += sx;
        }

        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}