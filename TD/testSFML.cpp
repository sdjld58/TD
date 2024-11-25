#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>

// 유틸리티 함수: 두 점 간의 거리 계산
float distance(sf::Vector2f a, sf::Vector2f b) {
    return std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

// 유닛 클래스 (수정 금지)
class Unit {
public:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    bool isBlinking = false; // 깜빡이는 상태 플래그
    float blinkTimer = 0.f;  // 깜빡이는 시간 추적

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

    // 깜빡이는 효과를 처리
    void updateBlink(float deltaTime) {
        if (isBlinking) {
            blinkTimer -= deltaTime;
            if (blinkTimer <= 0.f) {
                isBlinking = false;
                shape.setFillColor(sf::Color::Red); // 원래 색상으로 복원
            }
            else {
                // 깜빡이는 효과: 색상 전환
                shape.setFillColor(shape.getFillColor() == sf::Color::Red ? sf::Color::White : sf::Color::Red);
            }
        }
    }

    void startBlinking(float duration) {
        isBlinking = true;
        blinkTimer = duration;
    }
};

// 발사체 클래스
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

    // 고정된 타워 설정
    sf::Vector2f towerPosition(400.f, 300.f);
    sf::CircleShape tower(30.f);
    tower.setFillColor(sf::Color::Blue);
    tower.setPosition(towerPosition.x - tower.getRadius(), towerPosition.y - tower.getRadius());

    // 유닛 생성
    Unit unit(800.f, 250.f, -0.5f, 0.3f);

    // 발사체 리스트
    std::vector<Bullet> bullets;

    // 타이머 설정
    sf::Clock clock;
    float bulletCooldown = 1.0f; // 발사 간격 (초 단위)
    float timeSinceLastBullet = 0.f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // 시간 업데이트
        float deltaTime = clock.restart().asSeconds();
        timeSinceLastBullet += deltaTime;

        // 유닛 이동
        unit.move();

        // 깜빡이는 상태 업데이트
        unit.updateBlink(deltaTime);

        // 발사체 이동
        for (auto& bullet : bullets) {
            bullet.move();
        }

        // 타워에서 발사체 생성
        if (timeSinceLastBullet >= bulletCooldown) {
            sf::Vector2f targetPosition = unit.getPosition(); // 유닛의 위치
            bullets.emplace_back(towerPosition, targetPosition, 5.f); // 타워에서 발사
            timeSinceLastBullet = 0.f;
        }

        // 발사체와 유닛 충돌 감지
        bullets.erase(
            std::remove_if(bullets.begin(), bullets.end(),
                [&](Bullet& bullet) {
                    // 충돌 범위: 발사체 중심과 유닛 중심 간 거리 계산
                    float collisionRadius = unit.shape.getRadius() + bullet.shape.getRadius();
                    if (distance(bullet.getPosition(), unit.getPosition()) <= collisionRadius) {
                        std::cout << "Hit! Unit blinking...\n";
                        unit.startBlinking(0.5f); // 유닛 깜빡이기 시작 (0.5초)
                        return true; // 충돌한 발사체 제거
                    }
                    return false;
                }),
            bullets.end());

        // 화면 렌더링
        window.clear();
        window.draw(tower); // 타워 그리기
        window.draw(unit.shape); // 유닛 그리기
        for (auto& bullet : bullets) {
            window.draw(bullet.shape); // 발사체 그리기
        }
        window.display();
    }

    return 0;
}
