// Map.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

const int TileSize = 8;
const int MapWidth = 100;
const int MapHeight = 100;

class Entity;

class Map
{
public:
    Map(){};
    Map(int width, int height, int canWalkPercent = 57, int cellularAutoMataTimes = 3);
    void generate();

    int getTerrainGridType(sf::Vector2i pos) const 
    {
        if (pos.x < 0 || pos.x >= width || pos.y < 0 || pos.y >= height) return false;
        return terrainGrids[pos.x][pos.y];
    }

    Entity* getEntityAt(sf::Vector2i pos) const
    {
        if (pos.x < 0 || pos.x >= width || pos.y < 0 || pos.y >= height) return nullptr;
        return entityGrids[pos.x][pos.y];
    }

    void render(sf::RenderWindow& window);
    sf::Vector2i getRandomFloorTile() const;
    
    int getWidth() const {return width; }
    int getHeight() const { return height; }
    void setWidth(int width) { this->width = width; }
    void setHeight(int height) { this->height = height; }
    void setTerrainGridType(sf::Vector2i pos, int val) { terrainGrids[pos.x][pos.y] = val; }
    void setEntityAt (sf::Vector2i pos, Entity* entity) { entityGrids[pos.x][pos.y] = entity; }

private:
    int width, height, canWalkPercent, cellularAutoMataTimes;
    std::vector<std::vector<int>> terrainGrids;
    std::vector<std::vector<Entity*>> entityGrids;
    sf::RectangleShape shape;
};