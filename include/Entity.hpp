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
    void init();
    bool isDead() const { return hp <= 0; }

    int getDamage() const;

    float getRawDamage() const { return rawDamage; }
    int getTeam() const { return team; }

    void setTeam(int team) { this->team = team; }
    void setMaxHp(float maxHp) { this->maxHp = maxHp; }
    void setMaxMp(float maxMp) { this->maxMp = maxMp; }
    void setDamage(float damage) { this->rawDamage = damage; }

    void takeDamage(float rawDamage);
private:
    float maxHp = 100.0, maxMp = 100.0;
    float hp = 100.0, mp = 100.0;
    float rawDamage = 20.0;
    int team = 1;
};

class EntityManager
{
public:
    EntityManager();
    void setPosition(sf::Vector2i pos);
    void render(sf::RenderWindow& window);
    
    bool move(int dx, int dy, World& map, Entity* self);
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
protected:
 bool bump(int dx, int dy, World& world);
private:
    bool spawned = false;
};