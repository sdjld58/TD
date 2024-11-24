#include "Projectile.h"
#include "UI.h"
#include <cmath>

Projectile::Projectile(int startX, int startY, int targetX, int targetY, float speed, const sf::Texture& texture)
    : position(static_cast<float>(startX), static_cast<float>(startY)),
    targetPosition(static_cast<float>(targetX), static_cast<float>(targetY)),
    speed(speed)
{
    // 방향 계산
    sf::Vector2f delta = targetPosition - position;
    float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    direction = delta / length;

    // 스프라이트 설정
    sprite.setTexture(texture);
    sprite.setPosition(position);

    // 회전 각도 설정 (옵션)
    float angle = std::atan2(delta.y, delta.x) * 180 / 3.14159265f;
    sprite.setRotation(angle + 90); // 필요에 따라 조정
}

void Projectile::update(sf::Time deltaTime)
{
    // 위치 업데이트
    position += direction * speed * deltaTime.asSeconds();
    sprite.setPosition(position);
}

bool Projectile::hasReachedTarget() const
{
    // 목표 위치에 도달했는지 확인
    sf::Vector2f delta = targetPosition - position;
    float dotProduct = delta.x * direction.x + delta.y * direction.y;
    return dotProduct <= 0;
}

const sf::Sprite& Projectile::getSprite() const
{
    return sprite;
}
