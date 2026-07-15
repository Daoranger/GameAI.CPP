//
// Created by hoang on 7/14/2026.
//

#include "Wall.h"

Wall::Wall(sf::Vector2f start, sf::Vector2f end)
    : startPoint(start)
    , endPoint(end)
{
}

void Wall::render(sf::RenderWindow& window)
{
    std::array<sf::Vertex, 2> line =
    {
        sf::Vertex{startPoint, sf::Color::White},
        sf::Vertex{endPoint, sf::Color::White}
    };

    window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
}
