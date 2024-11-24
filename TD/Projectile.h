#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>

class Projectile
{
public:
    Projectile(int startX, int startY, int targetX, int targetY, float speed, const sf::Texture& texture);

    void update(sf::Time deltaTime);
    bool hasReachedTarget() const;
    const sf::Sprite& getSprite() const;

private:
    sf::Vector2f position;
    sf::Vector2f targetPosition;
    sf::Vector2f direction;
    float speed;
    sf::Sprite sprite;
};



#endif
