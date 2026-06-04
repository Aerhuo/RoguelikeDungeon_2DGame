#include "Entity.hpp"
#include "World.hpp"

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
    if (world.map.getTerrainGridType(toPos) == 0) return false; // 无法移动则取消行动

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

EntityManager::EntityManager()
{
    shape.setSize(sf::Vector2f(TileSize, TileSize));
    shape.setFillColor(sf::Color::Blue);
}

void EntityManager::setPosition(sf::Vector2i pos)
{
    this->pos = pos;
}

bool EntityManager::move(int dx, int dy, World& world, Entity* self)
{
    sf::Vector2i cpos = sf::Vector2i(pos.x + dx, pos.y + dy);

    Entity* toEntity = world.map.getEntityAt(cpos);

    // 仍然判断一次目标位置是否已经存在实体
    if (toEntity == nullptr)
    {
        // 可以直接通行
        world.map.setEntityAt(pos, nullptr);
        world.map.setEntityAt(cpos, self);
        pos = cpos;
        return true;
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