#include "Player.hpp"
#include "World.hpp"

Player::Player()
{
    data.setMaxHp(100.0), data.setMaxMp(100.0);
    data.setDamage(20.0);
    data.setTeam(1);
    data.init();

    manager.setColor(sf::Color::Blue);
}

void Player::updateAction(World& world)
{

}

void Player::dead(World& World, sf::RenderWindow& window)
{
    window.close();
}

// 玩家输入响应
bool Player::handleInput(int dx, int dy, World& world)
{
    return bump(dx, dy, world);
}