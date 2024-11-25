#include "TextBox.h"
#include "UI.h"


TextBox::TextBox(const UI &ui, bool isDefense, const std::vector<std::wstring>& strings)
    : windowWidth(ui.windowWidth), windowHeight(ui.windowHeight) {
    // isDefense�� ���� ��ġ �� ���� ����
    position = isDefense ? sf::Vector2f(5.f, windowHeight - 300.f) : sf::Vector2f(windowWidth - 605.f, 5.f);
    color = isDefense ? sf::Color(0, 102, 204) : sf::Color(204, 0, 0);

    // ��Ʈ �ε�
    if (!font.loadFromFile("resources/fonts/BMDOHYEON_ttf.ttf")) {
        throw std::runtime_error("Font loading failed");
    }

    // �ؽ�Ʈ ���� �� ��ġ ����
    float textY = position.y + 40.f;
    int initTextSize = 30;
    for (const auto& str : strings) {
        sf::Text text(str, font, initTextSize);
        text.setFillColor(sf::Color::White);
        sf::FloatRect bounds = text.getLocalBounds();
        text.setPosition(position.x + size.x / 2.f - bounds.width / 2.f, textY);
        texts.push_back(text);
        textY += 60.f; // ���� �ؽ�Ʈ ��ġ
        initTextSize = 22;
    }
}

void TextBox::draw(sf::RenderWindow& window) {
    // �ձ� �簢�� ��ü
    sf::RectangleShape body(sf::Vector2f(size.x - 2 * cornerRadius, size.y));
    body.setFillColor(color);
    body.setPosition(position.x + cornerRadius, position.y);

    // ���� �𼭸�
    sf::CircleShape topLeftCorner(cornerRadius);
    topLeftCorner.setFillColor(color);
    topLeftCorner.setPosition(position.x, position.y);

    sf::CircleShape bottomLeftCorner(cornerRadius);
    bottomLeftCorner.setFillColor(color);
    bottomLeftCorner.setPosition(position.x, position.y + size.y - 2 * cornerRadius);

    sf::CircleShape topRightCorner(cornerRadius);
    topRightCorner.setFillColor(color);
    topRightCorner.setPosition(position.x + size.x - 2 * cornerRadius, position.y);

    sf::CircleShape bottomRightCorner(cornerRadius);
    bottomRightCorner.setFillColor(color);
    bottomRightCorner.setPosition(position.x + size.x - 2 * cornerRadius, position.y + size.y - 2 * cornerRadius);

    // ���Ʒ� �� �簢��
    sf::RectangleShape verticalBody(sf::Vector2f(size.x, size.y - 2 * cornerRadius));
    verticalBody.setFillColor(color);
    verticalBody.setPosition(position.x, position.y + cornerRadius);

    // ȭ�鿡 �׸���
    window.draw(verticalBody);
    window.draw(body);
    window.draw(topLeftCorner);
    window.draw(bottomLeftCorner);
    window.draw(topRightCorner);
    window.draw(bottomRightCorner);

    // �ؽ�Ʈ �׸���
    for (const auto& text : texts) {
        window.draw(text);
    }
}
