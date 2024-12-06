#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "UI.h"

class UI;

class TextBox {
   
protected:

    int life;
    int gold;
    int attackGold;

    sf::Vector2f position;
    sf::Vector2f sidePosition = sf::Vector2f(0.f, 0.f);

    sf::Vector2f size = sf::Vector2f(500.f, 350.f); // ������ �ڽ� ũ��
    sf::Vector2f sideSize; // ������ ���̵� �ڽ� ũ��

 
    float cornerRadius = 40.f;                      // ������ �ձ� �𼭸� �ݰ�

    sf::Color color;
    sf::Color sideColor;
    sf::Font font;
    std::vector<sf::Text> texts;

    const int windowWidth;
    const int windowHeight;

public:
    TextBox(const UI &ui, bool isDefense, const std::vector<std::wstring>& strings);
    void draw(sf::RenderWindow& window);
    void sideDraw(sf::RenderWindow& window);
    void updateState(int playerlife, int gold, int attackgold);
};

#endif // TEXTBOX_H
