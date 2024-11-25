#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>

// ��ƿ��Ƽ �Լ�: �� �� ���� �Ÿ� ���
float distance(sf::Vector2f a, sf::Vector2f b) {
    return std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

// ���� Ŭ���� (���� ����)
class Unit {
public:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    bool isBlinking = false; // �����̴� ���� �÷���
    float blinkTimer = 0.f;  // �����̴� �ð� ����

    Unit(float x, float y, float vx, float vy) {
        shape.setRadius(15.f);
        shape.setFillColor(sf::Color::Red);
        shape.setPosition(x, y);

        velocity = sf::Vector2f(vx, vy);
    }

    void move() {
        shape.move(velocity);
    }

    sf::Vector2f getPosition() const {
        return shape.getPosition();
    }

    // �����̴� ȿ���� ó��
    void updateBlink(float deltaTime) {
        if (isBlinking) {
            blinkTimer -= deltaTime;
            if (blinkTimer <= 0.f) {
                isBlinking = false;
                shape.setFillColor(sf::Color::Red); // ���� �������� ����
            }
            else {
                // �����̴� ȿ��: ���� ��ȯ
                shape.setFillColor(shape.getFillColor() == sf::Color::Red ? sf::Color::White : sf::Color::Red);
            }
        }
    }

    void startBlinking(float duration) {
        isBlinking = true;
        blinkTimer = duration;
    }
};

// �߻�ü Ŭ����
class Bullet {
public:
    sf::CircleShape shape;
    sf::Vector2f velocity;

    Bullet(sf::Vector2f startPosition, sf::Vector2f targetPosition, float speed) {
        shape.setRadius(5.f);
        shape.setFillColor(sf::Color::Yellow);
        shape.setPosition(startPosition);

        sf::Vector2f direction = targetPosition - startPosition;
        float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (magnitude != 0) {
            velocity = direction / magnitude * speed;
        }
        else {
            velocity = sf::Vector2f(0.f, 0.f);
        }
    }

    void move() {
        shape.move(velocity);
    }

    sf::Vector2f getPosition() const {
        return shape.getPosition();
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tower Shooting with Blink");
    window.setFramerateLimit(60);

    // ������ Ÿ�� ����
    sf::Vector2f towerPosition(400.f, 300.f);
    sf::CircleShape tower(30.f);
    tower.setFillColor(sf::Color::Blue);
    tower.setPosition(towerPosition.x - tower.getRadius(), towerPosition.y - tower.getRadius());

    // ���� ����
    Unit unit(800.f, 250.f, -0.5f, 0.3f);

    // �߻�ü ����Ʈ
    std::vector<Bullet> bullets;

    // Ÿ�̸� ����
    sf::Clock clock;
    float bulletCooldown = 1.0f; // �߻� ���� (�� ����)
    float timeSinceLastBullet = 0.f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // �ð� ������Ʈ
        float deltaTime = clock.restart().asSeconds();
        timeSinceLastBullet += deltaTime;

        // ���� �̵�
        unit.move();

        // �����̴� ���� ������Ʈ
        unit.updateBlink(deltaTime);

        // �߻�ü �̵�
        for (auto& bullet : bullets) {
            bullet.move();
        }

        // Ÿ������ �߻�ü ����
        if (timeSinceLastBullet >= bulletCooldown) {
            sf::Vector2f targetPosition = unit.getPosition(); // ������ ��ġ
            bullets.emplace_back(towerPosition, targetPosition, 5.f); // Ÿ������ �߻�
            timeSinceLastBullet = 0.f;
        }

        // �߻�ü�� ���� �浹 ����
        bullets.erase(
            std::remove_if(bullets.begin(), bullets.end(),
                [&](Bullet& bullet) {
                    // �浹 ����: �߻�ü �߽ɰ� ���� �߽� �� �Ÿ� ���
                    float collisionRadius = unit.shape.getRadius() + bullet.shape.getRadius();
                    if (distance(bullet.getPosition(), unit.getPosition()) <= collisionRadius) {
                        std::cout << "Hit! Unit blinking...\n";
                        unit.startBlinking(0.5f); // ���� �����̱� ���� (0.5��)
                        return true; // �浹�� �߻�ü ����
                    }
                    return false;
                }),
            bullets.end());

        // ȭ�� ������
        window.clear();
        window.draw(tower); // Ÿ�� �׸���
        window.draw(unit.shape); // ���� �׸���
        for (auto& bullet : bullets) {
            window.draw(bullet.shape); // �߻�ü �׸���
        }
        window.display();
    }

    return 0;
}
