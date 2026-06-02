#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Map.hpp"
#include "Event.hpp"

struct World;

class EntityData
{
public:
    void init() { hp = maxHp, mp = maxMp; };
    bool isDead() const { return hp <= 0; }

    void setMaxHp(int maxHp) { this->maxHp = maxHp; }
    void setMaxMp(int maxMp) { this->maxMp = maxMp; }
private:
    int maxHp = 100, maxMp = 100;
    int hp = 100, mp = 100;
};

class EntityManager
{
public:
    EntityManager();
    void setPosition(sf::Vector2i pos);
    void render(sf::RenderWindow& window);
    
    bool move(int dx, int dy, Map& map, Entity* self);
    sf::Vector2i getPos() const;
    void setColor(sf::Color color) { shape.setFillColor(color); }

private:
    sf::Vector2i pos;
    sf::RectangleShape shape = sf::RectangleShape(sf::Vector2f(TileSize, TileSize));
};

class Entity
{
public:
    EntityData data;
    EntityManager manager;

    void spawn(Map& map);
    virtual ~Entity() = default;

    // 行动逻辑
    virtual void updateAction(World& world) = 0;
    virtual void dead(World& world, sf::RenderWindow& window) = 0;
private:
    bool spawned = false;
};