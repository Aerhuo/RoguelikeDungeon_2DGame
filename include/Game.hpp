#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Map.hpp"
#include "World.hpp"
#include "Enemy.hpp"
#include "Player.hpp"

const int PixelX = 720;
const int PixelY = 720;

class Game
{
public:
    Game();
    void run();

private:
    void processEvents();
    void update();
    void render();
    void executeTurn();
    
    sf::RenderWindow window;
    sf::View camera;

    World world;
    void runBFSFindRoad();
};