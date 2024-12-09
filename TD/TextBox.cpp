
#include "UI.h"


UI::TextBox::TextBox(const UI& ui, bool isDefense, const std::vector<std::wstring>& strings)
    : uip(&ui),windowWidth(ui.windowWidth), windowHeight(ui.windowHeight) {
    // isDefense�� ���� ��ġ �� ���� ����
    position = isDefense ? sf::Vector2f(15.f, windowHeight - 350.f) : sf::Vector2f(windowWidth - 500.f, 15.f);
    sideSize = isDefense ? sf::Vector2f(200.f, windowHeight) : sf::Vector2f(windowWidth, 160.f);
    color = isDefense ? sf::Color(0, 102, 204) : sf::Color(204, 0, 0);
    sideColor = isDefense ? sf::Color(0, 51, 153,190) : sf::Color(153, 0, 0,190);

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
        textY += 55.f; // ���� �ؽ�Ʈ ��ġ
        initTextSize = 23;
    }
}

void UI::TextBox::draw(sf::RenderWindow& window) {

    sideDraw(window);  //���̵� �� �׸���

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

void UI::TextBox::sideDraw(sf::RenderWindow& window) {
    // ���簢�� ��ü
    sf::RectangleShape body(sf::Vector2f(sideSize.x, sideSize.y));
    body.setFillColor(sideColor);
    body.setPosition(sidePosition);

    // �׵θ� �߰�
    float borderThickness = 5.0f; // �׵θ� �β�
    sf::Color borderColor = sf::Color::Black; // �׵θ� ����
    body.setOutlineThickness(borderThickness);
    body.setOutlineColor(borderColor);

    // ȭ�鿡 ��ü �׸���
    window.draw(body);

    // ��Ʈ �ε�
    sf::Font font;
    if (!font.loadFromFile("resources/fonts/Bangers.ttf")) {
        throw std::runtime_error("Font loading failed");
    }
    sf::Font fontbm;
    // ��Ʈ �ε�
    if (!fontbm.loadFromFile("resources/fonts/BMDOHYEON_ttf.ttf")) {
        throw std::runtime_error("Font loading failed");
    }

    // �⺻ �ؽ�Ʈ ����
    sf::Text lifeText("L i f e : " + std::to_string(life), font, 32);
    sf::Text goldText("G o l d : " + std::to_string(gold), font, 32);
    sf::Text attackGoldText("Attack Gold : " + std::to_string(attackGold), font, 27);

    // �ؽ�Ʈ ���� ����
    lifeText.setFillColor(sf::Color::White);
    goldText.setFillColor(sf::Color::White);
    attackGoldText.setFillColor(sf::Color::White);

    // �ؽ�Ʈ ��ġ ����
    float textStartX = sidePosition.x + 15.f; // �ؽ�Ʈ ���� X ��ġ
    float textStartY = sidePosition.y + 20.f; // �ؽ�Ʈ ���� Y ��ġ

    lifeText.setPosition(textStartX, textStartY);
    goldText.setPosition(textStartX, textStartY + 40.f);
    attackGoldText.setPosition(textStartX, textStartY + 80.f);

    // �ؽ�Ʈ �׸���
    window.draw(lifeText);
    window.draw(goldText);
    window.draw(attackGoldText);

    // ���� �̹����� ������� ���
    if (uip->curWaveIsDefense) {
        float currentY = textStartY + 250.f; // �⺻ �ؽ�Ʈ �Ʒ����� ����
        for (const auto& pair : uip->unitCounts) {
            int unitId = pair.first;
            int count = pair.second;

            sf::Sprite unitSprite;
            float scale = 0.5f; // �⺻ ũ��
            float yAdjustment = 0.f; // �⺻ Y ��ġ ���� ��

            // ���� ID�� ������� �̹��� ����
            switch (unitId) {
            case 1: unitSprite = uip->WolfSprite; unitSprite.setTexture(uip->WolfTexture[0]); break;
            case 2:
                unitSprite = uip->BatSprite;
                unitSprite.setTexture(uip->BatTexture[0]);
                scale = 0.8f; // ũ�⸦ �� ũ��
                yAdjustment = -10.f; // Y ��ġ ���� ����
                break;
            case 3: unitSprite = uip->BearSprite; unitSprite.setTexture(uip->BearTexture[0]); break;
            case 4: unitSprite = uip->PossessedWolfSprite; unitSprite.setTexture(uip->PossessedWolfTexture[0]); break;
            case 5:
                unitSprite = uip->MohawkBatSprite;
                unitSprite.setTexture(uip->MohawkBatTexture[0]);
                scale = 0.7f; // ũ�⸦ �� ũ��
                yAdjustment = -10.f; // Y ��ġ ���� ����
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

            // ��������Ʈ ũ�� �� ��ġ ����
            unitSprite.setScale(scale, scale);
            unitSprite.setPosition(textStartX + 50.f, currentY + yAdjustment);

            // ���� �ؽ�Ʈ ����
            sf::Text countText("x  " + std::to_string(count), font, 35);
            countText.setFillColor(sf::Color::White);
            countText.setPosition(textStartX + 110.f, currentY - 80); // �̹��� �����ʿ� ��ġ

            // �׸���
            window.draw(unitSprite);
            window.draw(countText);

            currentY += 80.f; // ���� �׸����� �̵�
        }
    }
    else {
        float currentX = textStartX + 230.f; // �⺻ �ؽ�Ʈ ���������� ����
        float currentY = textStartY + 130.f; // �ؽ�Ʈ �Ʒ����� ����

        for (int unitId = 1; unitId <= 9; ++unitId) {
            sf::Sprite unitSprite;
            float scale = 0.5f; // �⺻ ũ��
            float yAdjustment = 0.f; // Y ��ġ ����
            float xAdjustment = 25.f;
            std::wstring unitName;

            switch (unitId) {
            case 1:
                unitSprite = uip->WolfSprite;
                unitSprite.setTexture(uip->WolfTexture[0]);
                unitName = L"[1]: ����";
                break;
            case 2:
                unitSprite = uip->BatSprite;
                unitSprite.setTexture(uip->BatTexture[0]);
                unitName = L"[2]:����";
                scale = 0.8f;
                yAdjustment = -10.f;
                xAdjustment = 30.f;
                break;
            case 3:
                unitSprite = uip->BearSprite;
                unitSprite.setTexture(uip->BearTexture[0]);
                unitName = L"[3]:��";
                xAdjustment = 40.f;
                break;
            case 4:
                unitSprite = uip->PossessedWolfSprite;
                unitSprite.setTexture(uip->PossessedWolfTexture[0]); 
                unitName = L"[4]:���ֹ�������";
                xAdjustment = 90.f;
                break;
            case 5:
                unitSprite = uip->MohawkBatSprite;
                unitSprite.setTexture(uip->MohawkBatTexture[0]);
                unitName = L"[5]:���͹���";
                scale = 0.7f;
                yAdjustment = -10.f;
                xAdjustment = 60.f;
                break;
            case 6: 
                unitSprite = uip->PossessedBearSprite; 
                unitSprite.setTexture(uip->PossessedBearTexture[0]); 
                unitName = L"[6]:���ֹ�����";
                xAdjustment = 70.f;
                break;
            case 7:
                unitSprite = uip->BlueScorpionSprite;
                unitSprite.setTexture(uip->BlueScorpionTexture[0]);
                scale = 0.4f;
                unitName = L"[7]:Ǫ��������";
                xAdjustment = 60.f;
                break;
            case 8: 
                unitSprite = uip->WinterWolfSprite; 
                unitSprite.setTexture(uip->WinterWolfTexture[0]);
                unitName = L"[8]:��������";
                xAdjustment = 40.f;
                break;
            case 9: 
                unitSprite = uip->WinterBearSprite; 
                unitSprite.setTexture(uip->WinterBearTexture[0]);
                unitName = L"[9]:������";
                xAdjustment = 30.f;
                break;
           
            default: continue;
            }

            unitSprite.setScale(scale, scale);
            unitSprite.setPosition(currentX, currentY + yAdjustment);

            sf::Text nameText(unitName, fontbm, 20);
            nameText.setFillColor(sf::Color::White);
            nameText.setPosition(currentX-xAdjustment, currentY - 30.f); // �̹��� �Ʒ��� �̸� ǥ��

            window.draw(unitSprite);
            window.draw(nameText);

            currentX += 135.f; // ���� �̹��� ���������� �̵�
        }
    }
}

void UI::TextBox::updateState(int playerlife, int gold, int attackgold)
{
    this->life = playerlife;
    this->gold = gold;
    this->attackGold = attackgold;
}
