#include "UIManager.hpp"
#include "Entity.hpp"
#include "Game.hpp"

UIManager::UIManager(){}

void UIManager::bindPlayerData(const EntityData* data)
{
    playerData = data;
}

bool UIManager::init()
{
    if (!font.loadFromFile("assets/fonts/arial.ttf"))
    {
        return false;
    }

    rightPanel.setSize(sf::Vector2f(PixelX * .2f, PixelY * .8f));
    rightPanel.setPosition(sf::Vector2f(PixelX * .8f, .0f));
    rightPanel.setFillColor(sf::Color(40, 40, 40));

    bottomPanel.setSize(sf::Vector2f(PixelX, PixelY * .2f));
    bottomPanel.setPosition(.0f, PixelY * .8f);
    bottomPanel.setFillColor(sf::Color(20, 20, 20));

    hpText.setFont(font);
    hpText.setCharacterSize(20);
    hpText.setFillColor(sf::Color::Red);
    hpText.setPosition(PixelX * .8f + 10.0f, 15.0f);
    hpText.setString("HP");

    mpText.setFont(font);
    mpText.setCharacterSize(20);
    mpText.setFillColor(sf::Color::Blue);
    mpText.setPosition(PixelX * .8f + 10.0f, 55.0f);
    mpText.setString("MP");

    maxHPShape.setFillColor(sf::Color(60, 60, 60));
    maxHPShape.setSize(sf::Vector2f(150.0f, 15.0f));
    maxHPShape.setPosition(PixelX * .8f + 50.0f, 20.0f);

    maxMPShape.setFillColor(sf::Color(60, 60, 60));
    maxMPShape.setSize(sf::Vector2f(150.0f, 15.0f));
    maxMPShape.setPosition(PixelX * .8f + 50.0f, 60.0f);

    hpShape.setFillColor(sf::Color::Red);
    hpShape.setSize(sf::Vector2f(150.0f, 15.0f));
    hpShape.setPosition(PixelX * .8f + 50.0f, 20.0f);

    mpShape.setFillColor(sf::Color::Blue);
    mpShape.setSize(sf::Vector2f(150.0f, 15.0f));
    mpShape.setPosition(PixelX * .8f + 50.0f, 60.0f);

    return true;
}

void UIManager::update()
{
    if (playerData == nullptr) return;

    hpShape.setSize(sf::Vector2f(150.0f * (playerData->getHP() / playerData->getMaxHP()), 15.0f));
    mpShape.setSize(sf::Vector2f(150.0f * (playerData->getMP() / playerData->getMaxMP()), 15.0f));
}

void UIManager::render(sf::RenderWindow& window)
{
    sf::View view = window.getView();

    window.setView(window.getDefaultView());

    window.draw(rightPanel);
    window.draw(bottomPanel);

    window.draw(maxHPShape);
    window.draw(maxMPShape);
    window.draw(hpShape);
    window.draw(mpShape);

    window.draw(hpText);
    window.draw(mpText);

    window.setView(view);
}