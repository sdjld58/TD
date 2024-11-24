#include "Projectile.h"
#include "UI.h"
#include <cmath>

Projectile::Projectile(int startX, int startY, int targetX, int targetY, float speed, const sf::Texture& texture)
    : position(static_cast<float>(startX), static_cast<float>(startY)),
    targetPosition(static_cast<float>(targetX), static_cast<float>(targetY)),
    speed(speed)
{
    // ���� ���
    sf::Vector2f delta = targetPosition - position;
    float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    direction = delta / length;

    // ��������Ʈ ����
    sprite.setTexture(texture);
    sprite.setPosition(position);

    // ȸ�� ���� ���� (�ɼ�)
    float angle = std::atan2(delta.y, delta.x) * 180 / 3.14159265f;
    sprite.setRotation(angle + 90); // �ʿ信 ���� ����
}

void Projectile::update(sf::Time deltaTime)
{
    // ��ġ ������Ʈ
    position += direction * speed * deltaTime.asSeconds();
    sprite.setPosition(position);
}

bool Projectile::hasReachedTarget() const
{
    // ��ǥ ��ġ�� �����ߴ��� Ȯ��
    sf::Vector2f delta = targetPosition - position;
    float dotProduct = delta.x * direction.x + delta.y * direction.y;
    return dotProduct <= 0;
}

const sf::Sprite& Projectile::getSprite() const
{
    return sprite;
}
