#include "Game.hpp"
#include "Map.hpp"
#include <ctime>
#include <cstdlib>

int main()
{
    std::srand(static_cast<unsigned>(time(0)));

    Game game = Game(MapWidth, MapHeight);
    game.run();
}