#pragma once
#include <SFML/Graphics.hpp>

class EntityData;

class UIManager
{
public:
    UIManager();

    void bindPlayerData(const EntityData* data);

    bool init();

    void update();

    void render(sf::RenderWindow& window);

private:
    const EntityData* playerData = nullptr;

    sf::Font font;
    sf::Text hpText;
    sf::Text mpText;
    sf::RectangleShape hpShape;
    sf::RectangleShape maxHPShape;
    sf::RectangleShape mpShape;
    sf::RectangleShape maxMPShape;
    sf::RectangleShape rightPanel;
    sf::RectangleShape bottomPanel;
};