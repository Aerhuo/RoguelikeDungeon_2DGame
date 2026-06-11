// Map.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>

static const int TileSize = 32;
static const int MapWidth = 50;
static const int MapHeight = 50;

class Entity;

struct World;

enum class TileType
{
    WALL,
    FLOOR,
    STAIRS_DOWN
};

class Map
{
public:
    Map(){};
    void init(int width, int height, int canWalkPercent = 57, int cellularAutoMataTimes = 3);
    
    void generate();
    void render(sf::RenderWindow& window, const World& world);
    bool canMove(sf::Vector2i pos) const;
    sf::Vector2i getRandomFloorTile() const;
    
    // get器
    int getWidth() const {return width; }
    int getHeight() const { return height; }
    Entity* getEntityAt(sf::Vector2i pos) const
    {
        if (pos.x < 0 || pos.x >= width || pos.y < 0 || pos.y >= height) return nullptr;
        return entityGrids[pos.x][pos.y];
    }
    TileType getTerrainGridType(sf::Vector2i pos) const 
    {
        if (pos.x < 0 || pos.x >= width || pos.y < 0 || pos.y >= height) return TileType::WALL;
        return terrainGrids[pos.x][pos.y];
    }

    // set器
    void setWidth(int width) { this->width = width; }
    void setHeight(int height) { this->height = height; }
    void setTerrainGridType(sf::Vector2i pos, TileType type) { terrainGrids[pos.x][pos.y] = type; }
    void setEntityAt (sf::Vector2i pos, Entity* entity) { entityGrids[pos.x][pos.y] = entity; }

    std::vector<Entity*> getEntitiesInRadius(sf::Vector2i center, int radius, std::function<bool(Entity*)> filter);

private:
    int width, height, canWalkPercent, cellularAutoMataTimes;
    std::vector<std::vector<TileType>> terrainGrids;
    std::vector<std::vector<Entity*>> entityGrids;
    sf::RectangleShape shape;
};