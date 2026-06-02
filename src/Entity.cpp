#include "Entity.hpp"

void Entity::spawn(Map& map)
{
    if (!spawned)
    {
        spawned = true;
        map.setEntityAt(manager.getPos(), this);
    }
}

EntityManager::EntityManager()
{
    shape.setSize(sf::Vector2f(TileSize, TileSize));
    shape.setFillColor(sf::Color::Blue);
}

void EntityManager::setPosition(sf::Vector2i pos)
{
    this->pos = pos;
}

bool EntityManager::move(int dx, int dy, Map& map, Entity* self)
{
    sf::Vector2i cpos = sf::Vector2i(pos.x + dx, pos.y + dy);

    Entity* toEntity = map.getEntityAt(cpos);
    if (map.getTerrainGridType(cpos)) // 目标位置可通行
    {
        if (toEntity != nullptr)
        {
            // 遇到敌对生物

        }
        else
        {
            // 可以直接通行
            map.setEntityAt(pos, nullptr);
            map.setEntityAt(cpos, self);
            pos = cpos;
            return true;
        }
    }

    return false;
}

void EntityManager::render(sf::RenderWindow& window)
{
    shape.setPosition(sf::Vector2f((float)pos.x * TileSize, (float)pos.y * TileSize));
    window.draw(shape);
}

sf::Vector2i EntityManager::getPos() const
{
    return pos;
}