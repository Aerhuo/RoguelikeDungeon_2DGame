#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>

class World;

class FOVEngine
{
public:
    // 计算视野
    static void calculateFOV(sf::Vector2i center, sf::Vector2i dir, int radius, const std::function<void(sf::Vector2i)>& onVisable, World* world);

private:
    static void castRay(sf::Vector2i start, sf::Vector2i end, int radius2, const std::function<void(sf::Vector2i)>& onVisable, World* world);
};