//
// Created by hoang on 7/5/2026.
//

#ifndef STEERINGBEHAVIORS_OBSTACLE_H
#define STEERINGBEHAVIORS_OBSTACLE_H

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"

class Obstacle
{
public:
    Obstacle(float radius);
    void render(sf::RenderWindow& window);

    void setPosition(sf::Vector2f pos);

private:
    sf::Vector2f position_;
    float circle_radius_;
    float collision_radius_;
};



#endif //STEERINGBEHAVIORS_OBSTACLE_H
