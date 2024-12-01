#include "TextBox.h"
#include "UI.h"


TextBox::TextBox(const UI &ui, bool isDefense, const std::vector<std::wstring>& strings)
    : windowWidth(ui.windowWidth), windowHeight(ui.windowHeight) {
    // isDefense에 따라 위치 및 색상 설정
    position = isDefense ? sf::Vector2f(15.f, windowHeight - 350.f) : sf::Vector2f(windowWidth - 500.f, 15.f);
    sideSize= isDefense ?  sf::Vector2f(160.f, windowHeight) :  sf::Vector2f(windowWidth, 160.f);
    color = isDefense ? sf::Color(0, 102, 204) : sf::Color(204, 0, 0);
    sideColor = isDefense ? sf::Color(0, 51, 153) : sf::Color(153, 0, 0);

    // 폰트 로드
    if (!font.loadFromFile("resources/fonts/BMDOHYEON_ttf.ttf")) {
        throw std::runtime_error("Font loading failed");
    }

    // 텍스트 생성 및 위치 설정
    float textY = position.y + 40.f;
    int initTextSize = 30;
    for (const auto& str : strings) {
        sf::Text text(str, font, initTextSize);
        text.setFillColor(sf::Color::White);
        sf::FloatRect bounds = text.getLocalBounds();
        text.setPosition(position.x + size.x / 2.f - bounds.width / 2.f, textY);
        texts.push_back(text);
        textY += 55.f; // 다음 텍스트 위치
        initTextSize = 23;
    }
}

void TextBox::draw(sf::RenderWindow& window) {

    sideDraw(window);  //사이드 바 그리기
    
    // 둥근 사각형 본체
    sf::RectangleShape body(sf::Vector2f(size.x - 2 * cornerRadius, size.y));
    body.setFillColor(color);
    body.setPosition(position.x + cornerRadius, position.y);

    // 원형 모서리
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

    // 위아래 긴 사각형
    sf::RectangleShape verticalBody(sf::Vector2f(size.x, size.y - 2 * cornerRadius));
    verticalBody.setFillColor(color);
    verticalBody.setPosition(position.x, position.y + cornerRadius);

    // 화면에 그리기
    window.draw(verticalBody);
    window.draw(body);
    window.draw(topLeftCorner);
    window.draw(bottomLeftCorner);
    window.draw(topRightCorner);
    window.draw(bottomRightCorner);

    // 텍스트 그리기
    for (const auto& text : texts) {
        window.draw(text);
    }
   
   
}



void TextBox::sideDraw(sf::RenderWindow& window) {
    // 둥근 사각형 본체
    sf::RectangleShape body(sf::Vector2f(sideSize.x - 2 * cornerRadius, sideSize.y));
    body.setFillColor(sideColor);
    body.setPosition(sidePosition.x + cornerRadius, sidePosition.y);

    // 원형 모서리
    sf::CircleShape topLeftCorner(cornerRadius);
    topLeftCorner.setFillColor(sideColor);
    topLeftCorner.setPosition(sidePosition.x, sidePosition.y);

    sf::CircleShape bottomLeftCorner(cornerRadius);
    bottomLeftCorner.setFillColor(sideColor);
    bottomLeftCorner.setPosition(sidePosition.x, sidePosition.y + sideSize.y - 2 * cornerRadius);

    sf::CircleShape topRightCorner(cornerRadius);
    topRightCorner.setFillColor(sideColor);
    topRightCorner.setPosition(sidePosition.x + sideSize.x - 2 * cornerRadius, sidePosition.y);

    sf::CircleShape bottomRightCorner(cornerRadius);
    bottomRightCorner.setFillColor(sideColor);
    bottomRightCorner.setPosition(sidePosition.x + sideSize.x - 2 * cornerRadius, sidePosition.y + sideSize.y - 2 * cornerRadius);

    // 위아래 긴 사각형
    sf::RectangleShape verticalBody(sf::Vector2f(sideSize.x, sideSize.y - 2 * cornerRadius));
    verticalBody.setFillColor(sideColor);
    verticalBody.setPosition(sidePosition.x, sidePosition.y + cornerRadius);

    // 화면에 본체와 둥근 모서리 그리기
    window.draw(verticalBody);
    window.draw(body);
    window.draw(topLeftCorner);
    window.draw(bottomLeftCorner);
    window.draw(topRightCorner);
    window.draw(bottomRightCorner);

    // 텍스트 생성 및 그리기
    sf::Font font;
    if (!font.loadFromFile("resources/fonts/Bangers.ttf")) {
        throw std::runtime_error("Font loading failed");
    }

    sf::Text lifeText("L i f e : " + std::to_string(life), font, 30);
    sf::Text goldText("G o l d : " + std::to_string(gold), font, 30);
    sf::Text attackGoldText("Attack Gold : " + std::to_string(attackGold), font, 25);

    // 텍스트 색상 설정
    lifeText.setFillColor(sf::Color::White);
    goldText.setFillColor(sf::Color::White);
    attackGoldText.setFillColor(sf::Color::White);

    // 텍스트 위치 설정 (박스 안에서 중앙 정렬)
    float positionX = 23.f;
    float textStartY = 25.f; // 박스 내부의 상단 여백

    lifeText.setPosition(positionX, textStartY);
    goldText.setPosition(positionX, textStartY + 40.f);
    attackGoldText.setPosition(positionX, textStartY + 80.f);

    // 텍스트 그리기
    window.draw(lifeText);
    window.draw(goldText);
    window.draw(attackGoldText);

    // 추가로 texts 벡터에 있는 텍스트 그리기
    for (const auto& text : texts) {
        window.draw(text);
    }
}

void TextBox::updateState(int playerlife, int gold, int attackgold)
{
    this->life = playerlife;
    this->gold = gold;
    this->attackGold = attackgold;
}
