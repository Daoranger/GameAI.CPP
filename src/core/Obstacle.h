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
    sf::Vector2f getPosition() const { return position_; }
    float getCollisionRadius() const;
    void tag() { is_tagged = true; }
    void untag() { is_tagged = false; }
    bool isTagged() const { return is_tagged; }

private:
    bool is_tagged = false;
    sf::Vector2f position_;
    float circle_radius_;
    float collision_radius_;
};



#endif //STEERINGBEHAVIORS_OBSTACLE_H
