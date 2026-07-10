//
// Created by hoang on 7/5/2026.
//

#include "Obstacle.h"

#include "SFML/Graphics/CircleShape.hpp"

Obstacle::Obstacle(float radius)
    : circle_radius_(radius)
    , collision_radius_(radius * 3 / 2)
{
}

void Obstacle::render(sf::RenderWindow& window)
{
    sf::CircleShape circle(circle_radius_);
    circle.setOrigin(sf::Vector2f(circle_radius_, circle_radius_));
    circle.setPosition(position_);
    circle.setFillColor(sf::Color::Blue);
    circle.setOutlineColor(sf::Color::White);
    window.draw(circle);

    sf::CircleShape collision(collision_radius_);
    collision.setOrigin(sf::Vector2f(collision_radius_, collision_radius_));
    collision.setPosition(position_);
    collision.setFillColor(sf::Color::Transparent);
    collision.setOutlineColor(sf::Color::White);
    collision.setOutlineThickness(3.0f);
    window.draw(collision);
}

void Obstacle::setPosition(sf::Vector2f pos)
{
    position_ = pos;
}
