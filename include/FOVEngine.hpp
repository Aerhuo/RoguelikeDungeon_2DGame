#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include "Map.hpp"
#include "World.hpp"

class FOVEngine
{
public:
    // 计算视野
    template<typename Func>
    static void calculateFOV(sf::Vector2i center, sf::Vector2i dir, int radius, Func onVisible, World* world)
    {
        int startX = std::max(0, center.x - radius * (dir.x <= 0));
        int startY = std::max(0, center.y - radius * (dir.y <= 0));
        int endX = std::min(MapWidth - 1, center.x + radius * (dir.x >= 0));
        int endY = std::min(MapHeight - 1, center.y + radius * (dir.y >= 0));

        int radius2 = radius * radius;
        for (int x = startX; x <= endX; ++x)
        {
            castRay(center, sf::Vector2i(x, startY), radius2, onVisible, world);
            castRay(center, sf::Vector2i(x, endY), radius2, onVisible, world);
        }
        
        for (int y = startY; y <= endY; ++y)
        {
            castRay(center, sf::Vector2i(startX, y), radius2, onVisible, world);
            castRay(center, sf::Vector2i(endX, y), radius2, onVisible, world);
        }
    }

private:
    template<typename Func>
    static void castRay(sf::Vector2i start, sf::Vector2i end, int radius2, Func onVisible, World* world)
    {
        int x0 = start.x, y0 = start.y;
        int x1 = end.x, y1 = end.y;

        int dx = abs(x0 - x1), dy = abs(y0 - y1);
        int sx = (x0 < x1 ? 1 : -1);
        int sy = (y0 < y1 ? 1 : -1);

        int err = dx - dy;
        while (true)
        {
            if (x0 < 0 || x0 >= world->map.getWidth() || y0 < 0 || y0 >= world->map.getHeight()) break;

            int dist = (x0 - start.x) * (x0 - start.x) + (y0 - start.y) * (y0 - start.y);
            if (dist <= radius2)
            {
                onVisible(sf::Vector2i(x0, y0));
            }

            if (!world->map.canMove(sf::Vector2i(x0, y0))) break;
            if (sf::Vector2i(x0, y0) == end) break;

            int e2 = err * 2;
            if (e2 > -dy)
            {
                err -= dy;
                x0 += sx;
            }

            if (e2 < dx)
            {
                err += dx;
                y0 += sy;
            }
        }
    }
};