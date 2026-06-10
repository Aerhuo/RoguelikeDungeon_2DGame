#include "Game.hpp"
#include "EnemyFactory.hpp"
#include <queue>

Game::Game(int width, int height) : world(width, height), window(sf::VideoMode(PixelX, PixelY), "First Stage")
{
    window.setFramerateLimit(60);
    camera.setSize(PixelX, PixelY);

    world.init();

    // 生成怪物
    EnemyFactory::spawnEnemies(world, EnemyType::SLIME, 5);
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
        if (event.type == sf::Event::KeyPressed && world.player.data.canAct())
        {
            bool skip = false;
            int dx = 0, dy = 0;
            if (event.key.code == sf::Keyboard::W) dy = -1;
            if (event.key.code == sf::Keyboard::S) dy = 1;
            if (event.key.code == sf::Keyboard::A) dx = -1;
            if (event.key.code == sf::Keyboard::D) dx = 1;
            if (event.key.code == sf::Keyboard::Enter) skip = true;

            if (skip || dx != 0 || dy != 0)
            {
                if (skip || world.player.handleInput(dx, dy, world))
                {
                    world.player.updateAction(world);
                    runBFSFindRoad();
                }
            }
        }
    }
}

void Game::update()
{
    if (!world.eventQueue.empty())
    {
        resolveEvents();

        // 更新迷雾
        if (world.fovDirty)
        {
            world.player.fogManager.update(&world);
            world.fovDirty = false;
        }
    }
    
    while (!world.player.data.canAct())
    {
        // 出现新事件，先进行处理
        if (!world.eventQueue.empty())
        {
            break;
        }

        bool monsterActed = false;
        for (auto& enemy : world.enemies)
        {
            if (enemy->data.canAct())
            {
                enemy->updateAction(world);
                monsterActed = true;
                break; // 同时只允许一个怪物进行行动
            }
        }

        // 推进时间轴
        if (!monsterActed)
        {
            world.player.data.addEnergy();
            for (auto& enemy : world.enemies)
            {
                enemy->data.addEnergy();
            }
        }
    }

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

    // 摄像机跟随
    float pixelX = world.player.manager.getPos().x * TileSize + TileSize / 2.0f;
    float pixelY = world.player.manager.getPos().y * TileSize + TileSize / 2.0f;
    camera.setCenter(pixelX, pixelY);
}

// 渲染各种图层
void Game::render()
{
    window.clear();
    window.setView(camera);

    world.map.render(window, world);
    world.player.manager.render(window, world);

    for (int i = 0; i < (int)world.enemies.size(); ++i) world.enemies[i]->manager.render(window, world);

    window.display();
}

// 执行回合行为
void Game::resolveEvents()
{
    // 处理事件队列
    while (!world.eventQueue.empty())
    {
        Event ev = world.eventQueue.front();
        world.eventQueue.pop();

        if (ev.type == EventType::MOVE)
        {
            // 移动事件
            ev.actor->manager.move(ev.dx, ev.dy, world);
            if (ev.actor->data.isPlayer()) world.fovDirty = true;
        }
        else if (ev.type == EventType::ATTACK)
        {
            // 攻击事件
            ev.target->data.takeDamage(ev.actor->data.getDamage());
            if (ev.target->data.isDead()) world.map.setEntityAt(ev.target->manager.getPos(), nullptr);
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
                if (cx < 0 || cx >= world.map.getWidth() || cy < 0 || cy >= world.map.getHeight()) continue;
                if (world.dist[cx][cy] != 1e9) continue;
                if (world.map.getTerrainGridType(sf::Vector2i(cx, cy)) == 0) continue;

                world.dist[cx][cy] = world.dist[x][y] + 1;
                q.push({cx, cy});
            }
        }
    }
}