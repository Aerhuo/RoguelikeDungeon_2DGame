#include "Game.hpp"
#include <queue>

Game::Game() : world(MapWidth, MapHeight), window(sf::VideoMode(PixelX, PixelY), "First Stage")
{
    window.setFramerateLimit(60);
    camera.setSize(PixelX, PixelY);

    world.map.generate();
    
    // 生成玩家
    world.player.manager.setPosition(world.map.getRandomFloorTile());
    world.player.data.init();
    world.player.spawn(world.map);

    // 生成怪物
    world.enemies.push_back(std::make_unique<Slime>());

    world.enemies.back()->manager.setPosition(world.map.getRandomFloorTile());
    world.enemies.back()->data.init();
    world.enemies.back()->spawn(world.map);

    world.dist.resize(MapWidth, std::vector<int>(MapHeight));
}

void Game::run()
{
    while (window.isOpen())
    {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }

        // 玩家按键响应
        if (event.type == sf::Event::KeyPressed)
        {
            int dx = 0, dy = 0;
            if (event.key.code == sf::Keyboard::W) dy = -1;
            if (event.key.code == sf::Keyboard::S) dy = 1;
            if (event.key.code == sf::Keyboard::A) dx = -1;
            if (event.key.code == sf::Keyboard::D) dx = 1;

            if (dx != 0 || dy != 0)
            {
                if (world.player.handleInput(dx, dy, world))
                {
                    executeTurn();
                }
            }
        }
    }
}

void Game::update()
{
    if (world.player.data.isDead())
    {
        // 玩家死亡
        world.player.dead(world, window);
    }

    // 判断怪物死亡
    for (int i = 0; i < (int)world.enemies.size(); )
    {
        if (world.enemies[i]->data.isDead())
        {
            world.enemies[i]->dead(world, window);
            swap(world.enemies[i], world.enemies.back());
            world.enemies.pop_back();
        }
        else
        {
            ++i;
        }
    }

    float pixelX = world.player.manager.getPos().x * TileSize + TileSize / 2.0f;
    float pixelY = world.player.manager.getPos().y * TileSize + TileSize / 2.0f;
    camera.setCenter(pixelX, pixelY);
}

void Game::render()
{
    window.clear();
    window.setView(camera);

    world.map.render(window);
    world.player.manager.render(window);

    for (int i = 0; i < (int)world.enemies.size(); ++i) world.enemies[i]->manager.render(window);

    window.display();
}

// 执行回合行为
void Game::executeTurn()
{
    runBFSFindRoad();
    world.player.updateAction(world);

    // 怪物回合
    for (auto& enemy : world.enemies) enemy->updateAction(world);

    // 处理事件队列
    while (!world.eventQueue.empty())
    {
        Event ev = world.eventQueue.front();
        world.eventQueue.pop();

        if (ev.type == EventType::MOVE)
        {
            // 移动事件
            ev.actor->manager.move(ev.dx, ev.dy, world.map, ev.actor);
        }
        else if (ev.type == EventType::ATTACK)
        {
            // 攻击事件

        }
        else
        {

        }
    }
}

void Game::runBFSFindRoad()
{
    std::vector<std::vector<int>> dirs = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };

    for (int x = 0; x < MapWidth; ++x)
    {
        for (int y = 0; y < MapHeight; ++y)
        {
            world.dist[x][y] = 1e9;
        }
    }

    int px = world.player.manager.getPos().x, py = world.player.manager.getPos().y;
    world.dist[px][py] = 0;
    std::queue<std::pair<int, int>> q; q.push({px, py});

    while (!q.empty())
    {
        int len = q.size();
        for (int i = 0; i < len; ++i)
        {
            auto [x, y] = q.front();
            q.pop();

            for (auto& dir : dirs)
            {
                int cx = x + dir[0], cy = y + dir[1];
                if (cx < 0 || cx >= MapWidth || cy < 0 || cy >= MapHeight) continue;
                if (world.dist[cx][cy] != 1e9) continue;
                if (world.map.getTerrainGridType(sf::Vector2i(cx, cy)) == 0) continue;

                world.dist[cx][cy] = world.dist[x][y] + 1;
                q.push({cx, cy});
            }
        }
    }
}