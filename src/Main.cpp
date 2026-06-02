#include "Game.hpp"
#include <ctime>
#include <cstdlib>

int main()
{
    std::srand(static_cast<unsigned>(time(0)));

    Game game = Game();
    game.run();
}