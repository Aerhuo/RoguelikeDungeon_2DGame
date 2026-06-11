#pragma once
#include <vector>
#include <queue>
#include "Map.hpp"
#include "Event.hpp"
#include "Player.hpp" 
#include "Entity.hpp"
#include <queue>
#include <memory>

struct World
{
    Map map;
    std::vector<std::vector<int>> dist;
    std::queue<Event> eventQueue;

    // 使用智能指针存储基类为 Entity 的各种 Enemy
    std::vector<std::unique_ptr<Entity>> enemies;
    Player player;

    bool fovDirty = true;

    int currentFloor = 0;
    bool isChangingFloor = false;

    void generateNextFloor();

    void init();
    World();
};