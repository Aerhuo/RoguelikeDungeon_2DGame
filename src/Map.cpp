#include "Map.hpp"
#include "World.hpp"
#include <cstdlib>
#include <queue>

void Map::init(int width, int height, int canWalkPercent, int cellularAutoMataTimes)
{
    terrainGrids.clear(), entityGrids.clear();

    this->width = width, this->height = height;
    this->canWalkPercent = canWalkPercent;
    this->cellularAutoMataTimes = cellularAutoMataTimes;
    terrainGrids.resize(width, std::vector<TileType>(height));
    entityGrids.resize(width, std::vector<Entity*>(height));
    shape.setSize(sf::Vector2f(TileSize, TileSize));
}

bool Map::canMove(sf::Vector2i pos) const
{
    if (pos.x < 0 || pos.x >= MapWidth || pos.y < 0 || pos.y >= MapHeight)
    {
        return false;
    }

    return terrainGrids[pos.x][pos.y] != TileType::WALL;
}

void Map::generate()
{
    // 随机噪声函数
    auto initNoise = [&]()
    {
        for (int x = 1; x < width - 1; ++x)
        {
            for (int y = 1; y < height - 1; ++y)
            {
                if (rand() % 100 < canWalkPercent)
                {
                    terrainGrids[x][y] = TileType::FLOOR;
                }
            }
        }
    };

    // 元胞自动机函数
    auto runCellularAutomata = [&]()
    {
        // 保留原网格图
        std::vector<std::vector<TileType>> originalGrids = terrainGrids;

        // 定义八个方向
        std::vector<std::vector<int>> dirs = {
            {-1, -1}, {-1, 0}, {-1, 1},
            {0, -1}, {0, 1},
            {1, -1}, {1, 0}, {1, -1}
        };
        
        for (int x = 1; x < width - 1; ++x)
        {
            for (int y = 1; y < height - 1; ++y)
            {
                // 统计八个方向的墙壁数量
                int wallCount = 0;
                for (auto& dir : dirs)
                {
                    int cx = x + dir[0], cy = y + dir[1];

                    if (cx < 0 || cx >= width || cy < 0 || cy >= height)
                    {
                        continue;
                    }

                    if (originalGrids[cx][cy] == TileType::WALL)
                    {
                        wallCount++;
                    }
                }

                // 4-5原则
                if (originalGrids[x][y] == TileType::WALL)
                {
                    if (wallCount < 4) terrainGrids[x][y] = TileType::FLOOR;
                }
                else
                {
                    if (wallCount >= 5) terrainGrids[x][y] = TileType::WALL;
                }
            }
        }
    };

    // 洪泛填充函数
    auto runFloodFill = [&]()
    {
        // 定义八个方向
        std::vector<std::vector<int>> dirs = {
            {-1, 0}, {1, 0}, {0, -1}, {0, 1}
        };
        
        // 定义布尔数组用于检查地块是否被访问
        std::vector<bool> vis(width * height);

        // 定义可通行岛屿
        std::vector<std::vector<std::pair<int, int>>> islands;

        int idx = -1;

        for (int x = 1; x < width - 1; ++x)
        {
            for (int y = 1; y < height - 1; ++y)
            {
                if (vis[y * width + x] || !canMove(sf::Vector2i(x, y))) continue;

                // 成功寻找到未被填充的岛屿
                std::queue<std::pair<int, int>> q;
                q.push({x, y});

                vis[y * width + x] = 1;

                islands.push_back({});

                // BFS把该岛屿完全填充
                while (!q.empty())
                {
                    auto [cx, cy] = q.front();
                    q.pop();

                    islands.back().push_back({cx, cy});

                    for (auto& dir : dirs)
                    {
                        int nx = cx + dir[0], ny = cy + dir[1];

                        if (nx < 0 || nx >= width || ny < 0 || ny >= height) continue;
                        if (vis[ny * width + nx] || !canMove(sf::Vector2i(nx, ny))) continue;

                        q.push({nx, ny});
                        vis[ny * width + nx] = 1;
                    }
                }

                if (idx == -1 || islands.back().size() > islands[idx].size()) idx = (int)islands.size() - 1;
            }
        }

        // 将除最大岛屿外的其它岛屿全部清空
        for (int i = 0; i < (int)islands.size(); ++i)
        {
            if (i == idx) continue;

            for (auto& [x, y] : islands[i])
            {
                terrainGrids[x][y] = TileType::WALL;
            }
        }
    };

    // 执行函数
    initNoise();
    for (int i = 0; i < cellularAutoMataTimes; ++i) runCellularAutomata();
    runFloodFill();
}

void Map::render(sf::RenderWindow& window, const World& world)
{
    sf::View camera = window.getView();
    int startX = std::max(0, (int)((camera.getCenter().x - camera.getSize().x / 2.0f) / TileSize));
    int startY = std::max(0, (int)((camera.getCenter().y - camera.getSize().y / 2.0f) / TileSize));
    int endX = std::min(width - 1, (int)((camera.getCenter().x + camera.getSize().x / 2.0f) / TileSize) + 1);
    int endY = std::min(height - 1, (int)((camera.getCenter().y + camera.getSize().y / 2.0f) / TileSize) + 1);

    for (int x = startX; x <= endX; ++x)
    {
        for (int y = startY; y <= endY; ++y)
        {
            sf::Vector2i pos = sf::Vector2i(x, y);
            FogState state = world.player.fogManager.getStateAt(pos);
            shape.setPosition((sf::Vector2f)pos * (float)TileSize);

            if (state == FogState::UNSEEN)
            {
                shape.setFillColor(sf::Color::Black);
                window.draw(shape);
            }
            else
            {
                TileType type = world.map.getTerrainGridType(pos);
                if (type == TileType::FLOOR)
                {
                    shape.setFillColor(sf::Color::Green);
                }
                else if (type == TileType::STAIRS_DOWN)
                {
                    shape.setFillColor(sf::Color::White);
                }
                else
                {
                    shape.setFillColor(sf::Color(100, 100, 100));
                }

                window.draw(shape);

                if (state == FogState::EXPLORED)
                {
                    shape.setFillColor(sf::Color(0, 0, 0, 150));
                    window.draw(shape);
                }
            }
        }
    }
}

sf::Vector2i Map::getRandomFloorTile() const
{
    int times = 0;
    while (times < width * height)
    {
        sf::Vector2i pos;
        pos.x = rand() % width;
        pos.y = rand() % height;

        if (canMove(pos)) return pos;

        times++;
    }

    return sf::Vector2i(-1, -1);
}

std::vector<Entity*> Map::getEntitiesInRadius(sf::Vector2i center, int radius, std::function<bool(Entity*)> filter)
{
    int startX = std::max(0, center.x - radius);
    int endX = std::min(width - 1, center.x + radius);
    int startY = std::max(0, center.y - radius);
    int endY = std::min(height - 1, center.y + radius);

    std::vector<Entity*> res;
    for (int x = startX; x <= endX; ++x)
    {
        for (int y = startY; y <= endY; ++y)
        {
            int dx = center.x - x;
            int dy = center.y - y;

            if (dx * dx + dy * dy <= radius * radius)
            {
                // 在圆内
                if (filter(entityGrids[x][y]))
                {
                    // 符合对象
                    res.push_back(entityGrids[x][y]);
                }
            }
        }
    }

    return res;
}