#pragma once

class Entity;

enum class EventType
{
    MOVE,
    ATTACK,
    DEAD,
    CHECK
};

struct Event
{
    Entity* actor;
    Entity* target;
    EventType type;
    int dx, dy;
    float damage;
};