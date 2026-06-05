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

    // 战斗行为
    void takeDamage(float rawDamage);
    int getDamage() const;

    // 速度行为
    bool canAct() const { return energy >= ActionThreshold; }
    void consumeEnergy(int cost = ActionThreshold) { energy -= cost; }
    void addEnergy() { energy += speed; }

    // get器
    float getHp() const { return hp; }
    float getMaxHp() const { return maxHp; }
    float getRawDamage() const { return rawDamage; }
    int getTeam() const { return team; }
    int getSpeed() const { return speed; }
    Entity* getOwner() const { return owner; }

    // set器
    void setTeam(int team) { this->team = team; }
    void setMaxHp(float maxHp) { this->maxHp = maxHp; }
    void setMaxMp(float maxMp) { this->maxMp = maxMp; }
    void setDamage(float damage) { this->rawDamage = damage; }
    void setSpeed(int speed) { this->speed = speed; }
    void setOwner(Entity* entity) { this->owner = entity; }

private:
    float maxHp = 100.0, maxMp = 100.0;
    float hp = 100.0, mp = 100.0;
    float rawDamage = 20.0;
    int team = 1;

    int speed = 10;
    int energy = 0;
    int fov = 5;

    static const int ActionThreshold = 100;

    Entity* owner;
};

class EntityManager
{
public:
    EntityManager();

    void render(sf::RenderWindow& window);
    
    bool move(int dx, int dy, World& map);

    // get器
    sf::Vector2i getDir() const { return dir; }
    sf::Vector2i getPos() const;
    Entity* getOwner() const { return owner; }

    // set器
    void setDir(sf::Vector2i dir) { this->dir = dir; }
    void setColor(sf::Color color) { shape.setFillColor(color); }
    void setPosition(sf::Vector2i pos);
    void setOwner(Entity* entity) { this->owner = entity; }

private:
    sf::Vector2i pos;
    sf::RectangleShape shape = sf::RectangleShape(sf::Vector2f(TileSize, TileSize));
    sf::Vector2i dir = {0, 0};
    
    Entity* owner;
};

class Entity
{
public:
    Entity();

    EntityData data;
    EntityManager manager;

    void spawn(Map& map);
    virtual ~Entity() = default;

    // 行动逻辑
    virtual void updateAction(World& world) = 0;
    virtual void dead(World& world, sf::RenderWindow& window) = 0;

    // get器

    // set器
protected:
    bool bump(int dx, int dy, World& world);
private:
    bool spawned = false;
};