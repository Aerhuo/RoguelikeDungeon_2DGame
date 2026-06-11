#pragma once
#include <SFML/Graphics.hpp>
#include <deque>

class EntityData;

class Event;

class UIManager
{
public:
    UIManager();

    void bindPlayerData(const EntityData* data);

    bool init();

    void update();

    void render(sf::RenderWindow& window);

    void triggerEvent(Event& event);

    const int maxMessagesCount = 5;

private:
    const EntityData* playerData = nullptr;

    sf::Font font;
    sf::Text hpText;
    sf::Text mpText;
    sf::Text messageText;
    sf::RectangleShape hpShape;
    sf::RectangleShape maxHPShape;
    sf::RectangleShape mpShape;
    sf::RectangleShape maxMPShape;
    sf::RectangleShape rightPanel;
    sf::RectangleShape bottomPanel;

    std::deque<std::wstring> messages;
};