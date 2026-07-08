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
    circle.setPosition(position_);
    circle.setFillColor(sf::Color::Blue);
    circle.setOutlineColor(sf::Color::White);
    window.draw(circle);

    sf::CircleShape collision(collision_radius_);
    collision.setPosition(position_);
    collision.setFillColor(sf::Color::Transparent);
    collision.setOutlineColor(sf::Color::White);
    window.draw(collision);
}

void Obstacle::setPosition(sf::Vector2f pos)
{
    position_ = pos;
}
