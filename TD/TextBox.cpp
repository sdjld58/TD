
#include "UI.h"


UI::TextBox::TextBox(const UI& ui, bool isDefense, const std::vector<std::wstring>& strings)
    : uip(&ui),windowWidth(ui.windowWidth), windowHeight(ui.windowHeight) {
    // isDefense에 따라 위치 및 색상 설정
    position = isDefense ? sf::Vector2f(15.f, windowHeight - 350.f) : sf::Vector2f(windowWidth - 500.f, 15.f);
    sideSize = isDefense ? sf::Vector2f(200.f, windowHeight) : sf::Vector2f(windowWidth, 160.f);
    color = isDefense ? sf::Color(0, 102, 204) : sf::Color(204, 0, 0);
    sideColor = isDefense ? sf::Color(0, 51, 153,190) : sf::Color(153, 0, 0,190);

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

void UI::TextBox::draw(sf::RenderWindow& window) {

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

void UI::TextBox::sideDraw(sf::RenderWindow& window) {
    // 직사각형 본체
    sf::RectangleShape body(sf::Vector2f(sideSize.x, sideSize.y));
    body.setFillColor(sideColor);
    body.setPosition(sidePosition);

    // 테두리 추가
    float borderThickness = 5.0f; // 테두리 두께
    sf::Color borderColor = sf::Color::Black; // 테두리 색상
    body.setOutlineThickness(borderThickness);
    body.setOutlineColor(borderColor);

    // 화면에 본체 그리기
    window.draw(body);

    // 폰트 로드
    sf::Font font;
    if (!font.loadFromFile("resources/fonts/Bangers.ttf")) {
        throw std::runtime_error("Font loading failed");
    }
    sf::Font fontbm;
    // 폰트 로드
    if (!fontbm.loadFromFile("resources/fonts/BMDOHYEON_ttf.ttf")) {
        throw std::runtime_error("Font loading failed");
    }

    // 기본 텍스트 생성
    sf::Text lifeText("L i f e : " + std::to_string(life), font, 32);
    sf::Text goldText("G o l d : " + std::to_string(gold), font, 32);
    sf::Text attackGoldText("Attack Gold : " + std::to_string(attackGold), font, 27);

    // 텍스트 색상 설정
    lifeText.setFillColor(sf::Color::White);
    goldText.setFillColor(sf::Color::White);
    attackGoldText.setFillColor(sf::Color::White);

    // 텍스트 위치 설정
    float textStartX = sidePosition.x + 15.f; // 텍스트 시작 X 위치
    float textStartY = sidePosition.y + 20.f; // 텍스트 시작 Y 위치

    lifeText.setPosition(textStartX, textStartY);
    goldText.setPosition(textStartX, textStartY + 40.f);
    attackGoldText.setPosition(textStartX, textStartY + 80.f);

    // 텍스트 그리기
    window.draw(lifeText);
    window.draw(goldText);
    window.draw(attackGoldText);

    // 유닛 이미지를 기반으로 출력
    if (uip->curWaveIsDefense) {
        float currentY = textStartY + 250.f; // 기본 텍스트 아래부터 시작
        for (const auto& pair : uip->unitCounts) {
            int unitId = pair.first;
            int count = pair.second;

            sf::Sprite unitSprite;
            float scale = 0.5f; // 기본 크기
            float yAdjustment = 0.f; // 기본 Y 위치 조정 값

            // 유닛 ID를 기반으로 이미지 선택
            switch (unitId) {
            case 1: unitSprite = uip->WolfSprite; unitSprite.setTexture(uip->WolfTexture[0]); break;
            case 2:
                unitSprite = uip->BatSprite;
                unitSprite.setTexture(uip->BatTexture[0]);
                scale = 0.8f; // 크기를 더 크게
                yAdjustment = -10.f; // Y 위치 위로 조정
                break;
            case 3: unitSprite = uip->BearSprite; unitSprite.setTexture(uip->BearTexture[0]); break;
            case 4: unitSprite = uip->PossessedWolfSprite; unitSprite.setTexture(uip->PossessedWolfTexture[0]); break;
            case 5:
                unitSprite = uip->MohawkBatSprite;
                unitSprite.setTexture(uip->MohawkBatTexture[0]);
                scale = 0.7f; // 크기를 더 크게
                yAdjustment = -10.f; // Y 위치 위로 조정
                break;
            case 6: unitSprite = uip->PossessedBearSprite; unitSprite.setTexture(uip->PossessedBearTexture[0]); break;
            case 7:
                unitSprite = uip->BlueScorpionSprite;
                unitSprite.setTexture(uip->BlueScorpionTexture[0]);
                scale = 0.4f;
                break;
            case 8: unitSprite = uip->WinterWolfSprite; unitSprite.setTexture(uip->WinterWolfTexture[0]); break;
            case 9: unitSprite = uip->WinterBearSprite; unitSprite.setTexture(uip->WinterBearTexture[0]); break;
            case 10:
                unitSprite = uip->JuggernautSprite;
                unitSprite.setTexture(uip->JuggernautTexture[0]);
                yAdjustment = 5.0f;
                break;
            case 11:
                unitSprite = uip->TrollSprite;
                unitSprite.setTexture(uip->TrollTexture[0]);
                yAdjustment = 5.0f;
                break;
            case 12:
                unitSprite = uip->WinterOgreSprite;
                unitSprite.setTexture(uip->WinterOgreTexture[0]);
                yAdjustment = 5.0f;
                break;
            default: continue;
            }

            // 스프라이트 크기 및 위치 설정
            unitSprite.setScale(scale, scale);
            unitSprite.setPosition(textStartX + 50.f, currentY + yAdjustment);

            // 개수 텍스트 생성
            sf::Text countText("x  " + std::to_string(count), font, 35);
            countText.setFillColor(sf::Color::White);
            countText.setPosition(textStartX + 110.f, currentY - 80); // 이미지 오른쪽에 배치

            // 그리기
            window.draw(unitSprite);
            window.draw(countText);

            currentY += 80.f; // 다음 항목으로 이동
        }
    }
    else {
        float currentX = textStartX + 230.f; // 기본 텍스트 오른쪽으로 시작
        float currentY = textStartY + 130.f; // 텍스트 아래에서 시작

        for (int unitId = 1; unitId <= 9; ++unitId) {
            sf::Sprite unitSprite;
            float scale = 0.5f; // 기본 크기
            float yAdjustment = 0.f; // Y 위치 조정
            float xAdjustment = 25.f;
            std::wstring unitName;

            switch (unitId) {
            case 1:
                unitSprite = uip->WolfSprite;
                unitSprite.setTexture(uip->WolfTexture[0]);
                unitName = L"[1]: 늑대";
                break;
            case 2:
                unitSprite = uip->BatSprite;
                unitSprite.setTexture(uip->BatTexture[0]);
                unitName = L"[2]:박쥐";
                scale = 0.8f;
                yAdjustment = -10.f;
                xAdjustment = 30.f;
                break;
            case 3:
                unitSprite = uip->BearSprite;
                unitSprite.setTexture(uip->BearTexture[0]);
                unitName = L"[3]:곰";
                xAdjustment = 40.f;
                break;
            case 4:
                unitSprite = uip->PossessedWolfSprite;
                unitSprite.setTexture(uip->PossessedWolfTexture[0]); 
                unitName = L"[4]:저주받은늑대";
                xAdjustment = 90.f;
                break;
            case 5:
                unitSprite = uip->MohawkBatSprite;
                unitSprite.setTexture(uip->MohawkBatTexture[0]);
                unitName = L"[5]:갈귀박쥐";
                scale = 0.7f;
                yAdjustment = -10.f;
                xAdjustment = 60.f;
                break;
            case 6: 
                unitSprite = uip->PossessedBearSprite; 
                unitSprite.setTexture(uip->PossessedBearTexture[0]); 
                unitName = L"[6]:저주받은곰";
                xAdjustment = 70.f;
                break;
            case 7:
                unitSprite = uip->BlueScorpionSprite;
                unitSprite.setTexture(uip->BlueScorpionTexture[0]);
                scale = 0.4f;
                unitName = L"[7]:푸른색전갈";
                xAdjustment = 60.f;
                break;
            case 8: 
                unitSprite = uip->WinterWolfSprite; 
                unitSprite.setTexture(uip->WinterWolfTexture[0]);
                unitName = L"[8]:설원늑대";
                xAdjustment = 40.f;
                break;
            case 9: 
                unitSprite = uip->WinterBearSprite; 
                unitSprite.setTexture(uip->WinterBearTexture[0]);
                unitName = L"[9]:설원곰";
                xAdjustment = 30.f;
                break;
           
            default: continue;
            }

            unitSprite.setScale(scale, scale);
            unitSprite.setPosition(currentX, currentY + yAdjustment);

            sf::Text nameText(unitName, fontbm, 20);
            nameText.setFillColor(sf::Color::White);
            nameText.setPosition(currentX-xAdjustment, currentY - 30.f); // 이미지 아래에 이름 표시

            window.draw(unitSprite);
            window.draw(nameText);

            currentX += 135.f; // 다음 이미지 오른쪽으로 이동
        }
    }
}

void UI::TextBox::updateState(int playerlife, int gold, int attackgold)
{
    this->life = playerlife;
    this->gold = gold;
    this->attackGold = attackgold;
}
