#include "UIManager.hpp"
#include "Entity.hpp"
#include "Game.hpp"
#include "Event.hpp"

UIManager::UIManager(){}

void UIManager::bindPlayerData(const EntityData* data)
{
    playerData = data;
}

bool UIManager::init()
{
    if (!font.loadFromFile("assets/fonts/MSYH.TTC"))
    {
        return false;
    }

    rightPanel.setSize(sf::Vector2f(PixelX * .2f, PixelY * .8f));
    rightPanel.setPosition(sf::Vector2f(PixelX * .8f, .0f));
    rightPanel.setFillColor(sf::Color(40, 40, 40));

    bottomPanel.setSize(sf::Vector2f(PixelX, PixelY * .2f));
    bottomPanel.setPosition(.0f, PixelY * .8f);
    bottomPanel.setFillColor(sf::Color(20, 20, 20));

    messageText.setFont(font);
    messageText.setCharacterSize(20);
    messageText.setFillColor(sf::Color::Yellow);

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

    while (messages.size() > maxMessagesCount) messages.pop_front();

    int idx = 0;
    for (auto& text : messages)
    {
        messageText.setPosition(sf::Vector2f(20.0f, PixelY * .8f + 10.0f + 25.0f * idx));
        messageText.setString(text);
        window.draw(messageText);
        idx++;
    }

    window.setView(view);
}

void UIManager::triggerEvent(Event& event)
{
    switch (event.type)
    {
        case EventType::ATTACK:
        {
            messages.push_back(event.actor->data.getName() + L"攻击了" + event.target->data.getName() + L", 并造成了" + std::to_wstring((int)event.damage) + L"点伤害！");
            break;
        }
        case EventType::MOVE:
        {
            break;
        }
        case EventType::DEAD:
        {
            messages.push_back(event.actor->data.getName() + L"死了！");
            break;
        }
        default: break;
    }
}