#pragma once

class Entity;

enum class EventType
{
    MOVE,
    ATTACK
};

struct Event
{
    Entity* actor;
    Entity* target;
    EventType type;
    int dx, dy;
};