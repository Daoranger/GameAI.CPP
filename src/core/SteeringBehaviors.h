//
// Created by hoang on 6/3/2026.
//

#ifndef STEERINGBEHAVIORS_STEERINGBEHAVIORS_H
#define STEERINGBEHAVIORS_STEERINGBEHAVIORS_H

#include "Obstacle.h"
#include "SFML/System/Vector2.hpp"

class Vehicle;

class SteeringBehaviors
{
public:
    SteeringBehaviors(Vehicle& vehicle);

    sf::Vector2f seek(sf::Vector2f target) const;
    sf::Vector2f flee(sf::Vector2f target) const;
    sf::Vector2f arrive(sf::Vector2f target) const;
    sf::Vector2f pursuit(const Vehicle& evader) const;
    sf::Vector2f evade(const Vehicle& pursuer) const;
    sf::Vector2f wander();
    sf::Vector2f obstacleAvoidance(const std::vector<std::unique_ptr<Obstacle>>& obstacles);

    float wanderRadius = 30.0f;
    float wanderDistance = 100.0f;
    float wanderJitter = 1.0f;
    sf::Vector2f wanderTarget;

private:
    Vehicle& vehicle_;
    sf::Vector2f pointToWorldSpace(sf::Vector2f targetLocal);
    sf::Vector2f pointToLocalSpace(sf::Vector2f targetWorld);
    sf::Vector2f vectorToWorldSpace(sf::Vector2f vecLocal);
    void tagObstaclesInRange(const std::vector<std::unique_ptr<Obstacle>>& obstacles, float range);

};

#endif //STEERINGBEHAVIORS_STEERINGBEHAVIORS_H
