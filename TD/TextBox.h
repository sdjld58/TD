#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "UI.h"

class UI;

class TextBox {
protected:
    sf::Vector2f position;
    sf::Vector2f size = sf::Vector2f(600.f, 400.f); // ������ �ڽ� ũ��
    float cornerRadius = 40.f;                      // ������ �ձ� �𼭸� �ݰ�
    sf::Color color;
    sf::Font font;
    std::vector<sf::Text> texts;

    const int windowWidth;
    const int windowHeight;

public:
    TextBox(const UI &ui, bool isDefense, const std::vector<std::wstring>& strings);
    void draw(sf::RenderWindow& window);
};

#endif // TEXTBOX_H
