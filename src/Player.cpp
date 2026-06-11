#include "Player.hpp"
#include "World.hpp"
#include "Map.hpp"
#include "Event.hpp"

Player::Player() : fogManager(this)
{
    data.setName(L"你");
    data.setMaxHp(100.0), data.setMaxMp(100.0);
    data.setDamage(20.0);
    data.setTeam(1);
    data.setSpeed(20);
    data.setFOV(7);
    data.init();

    manager.setColor(sf::Color::Blue);
}

void Player::updateAction(World& world)
{
    data.consumeEnergy();
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