#include "Player.hpp"
#include "World.hpp"

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
    sf::Vector2i toPos = sf::Vector2i(world.player.manager.getPos().x + dx, world.player.manager.getPos().y + dy);
    if (world.map.getTerrainGridType(toPos) == 0) return false; // 无法移动则取消行动

    Event ev;
    ev.dx = dx, ev.dy = dy;
    ev.actor = &world.player;
    if (world.map.getTerrainGridType(toPos) == 1)
    {
        // 如果目标位置可通行且不存在敌人
        ev.type = EventType::MOVE;
        world.eventQueue.push(ev);
    }
    else
    {
        // 如果目标位置存在敌人，则触发攻击事件
        ev.type = EventType::ATTACK;
        world.eventQueue.push(ev);
    }

    return true;
}