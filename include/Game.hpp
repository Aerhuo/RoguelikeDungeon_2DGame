#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Map.hpp"
#include "World.hpp"
#include "Enemy.hpp"
#include "Player.hpp"
#include "UIManager.hpp"

const int PixelX = 1080;
const int PixelY = 720;

class Game
{
public:
    Game(int width, int height);
    void run();

private:
    void processEvents();
    void update();
    void render();
    void resolveEvents();
    
    sf::RenderWindow window;
    sf::View camera;
    UIManager ui;

    World world;
    void runBFSFindRoad();
};