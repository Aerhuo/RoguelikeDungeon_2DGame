#include "Entity.hpp"
#include "World.hpp"
#include "FogManager.hpp"
#include "Map.hpp"
#include "Event.hpp"

int EntityData::getDamage() const
{
    return rawDamage;
}

void EntityData::takeDamage(float rawDamage)
{
    hp -= rawDamage;
}

void EntityData::init()
{
    hp = maxHp, mp = maxMp;
}

EntityManager::EntityManager() : shape(sf::Vector2f(TileSize, TileSize))
{
    shape.setSize(sf::Vector2f(TileSize, TileSize));
    shape.setFillColor(sf::Color::Blue);
}

Entity::Entity()
{
    data.setOwner(this);
    manager.setOwner(this);
}

void Entity::spawn(Map& map)
{
    if (!spawned)
    {
        spawned = true;
        map.setEntityAt(manager.getPos(), this);
    }
}

bool Entity::bump(int dx, int dy, World& world)
{
    sf::Vector2i toPos = sf::Vector2i(manager.getPos().x + dx, manager.getPos().y + dy);
    manager.setDir(sf::Vector2i{dx, dy});
    if (!world.map.canMove(toPos)) return false; // 无法移动则取消行动

    Entity* enemy = world.map.getEntityAt(toPos);
    Event ev;
    ev.dx = dx, ev.dy = dy;
    ev.actor = this;
    
    if (enemy != nullptr && enemy->data.getTeam() != data.getTeam())
    {
        // 如果目标位置存在敌人，则触发攻击事件
        ev.type = EventType::ATTACK;
        ev.target = enemy;
    }
    else
    {
        // 如果目标位置可通行且不存在敌人
        ev.type = EventType::MOVE;
    }

    world.eventQueue.push(ev);

    return true;
}

void EntityManager::setPosition(sf::Vector2i pos)
{
    this->pos = pos;
}

bool EntityManager::move(int dx, int dy, World& world)
{
    sf::Vector2i cpos = sf::Vector2i(pos.x + dx, pos.y + dy);
    Entity* toEntity = world.map.getEntityAt(cpos);

    // 仍然判断一次目标位置是否已经存在实体
    if (toEntity == nullptr)
    {
        // 可以直接通行
        world.map.setEntityAt(pos, nullptr);
        world.map.setEntityAt(cpos, owner);
        pos = cpos;
        return true;
    }

    return false;
}

void EntityManager::render(sf::RenderWindow& window, const World& world)
{
    if (world.player.fogManager.getStateAt(pos) != FogState::VISIBLE) return;
    sf::View camera = window.getView();
    int startX = std::max(0, (int)((camera.getCenter().x - camera.getSize().x / 2.0f) / TileSize));
    int startY = std::max(0, (int)((camera.getCenter().y - camera.getSize().y / 2.0f) / TileSize));
    int endX = std::min(world.map.getWidth() - 1, (int)((camera.getCenter().x + camera.getSize().x / 2.0f) / TileSize) + 1);
    int endY = std::min(world.map.getHeight() - 1, (int)((camera.getCenter().y + camera.getSize().y / 2.0f) / TileSize) + 1);

    if (startX <= pos.x && pos.x <= endX && startY <= pos.y && pos.y <= endY)
    {
        shape.setPosition(sf::Vector2f((float)pos.x * TileSize, (float)pos.y * TileSize));
        window.draw(shape);
    }
}

sf::Vector2i EntityManager::getPos() const
{
    return pos;
}