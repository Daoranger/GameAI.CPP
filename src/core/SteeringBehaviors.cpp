#include "SteeringBehaviors.h"
#include "Vehicle.h"
#include "Utils.h"
#include <limits>

SteeringBehaviors::SteeringBehaviors(Vehicle& vehicle)
    : vehicle_(vehicle)
    , wanderTarget(wanderRadius, 0)
{
}

sf::Vector2f SteeringBehaviors::seek(sf::Vector2f target) const
{
    sf::Vector2f desired_velocity = (target - vehicle_.position).normalized() * vehicle_.maxSpeed;
    return desired_velocity - vehicle_.velocity;
}

sf::Vector2f SteeringBehaviors::flee(sf::Vector2f target) const
{
    sf::Vector2f desired_velocity = (vehicle_.position - target).normalized() * vehicle_.maxSpeed;
    return desired_velocity - vehicle_.velocity;
}

sf::Vector2f SteeringBehaviors::arrive(sf::Vector2f target) const
{
    sf::Vector2f toTarget = target - vehicle_.position;
    float dist = toTarget.length();

    if (dist > 0)
    {
        const float DECELERATION_TWEAKER = 0.3; // because deceleration is integer, need this value to tweak deceleration
        const int deceleration = 1;             // slow = 3, normal = 2, fast = 1

        // this is one of many way to get the arrive speed
        float speed = dist / deceleration * DECELERATION_TWEAKER;
        
        speed = std::min(speed, vehicle_.maxSpeed);

        sf::Vector2f desired_velocity = toTarget * speed / dist;

        return desired_velocity - vehicle_.velocity;
    }

    // stop when reached target
    return sf::Vector2f(0,0);
}

sf::Vector2f SteeringBehaviors::pursuit(const Vehicle& evader) const
{
    // vector from pursuer to evader
    sf::Vector2f toEvader = evader.position - vehicle_.position;

    double relativeHeading = vehicle_.heading().dot(evader.heading());
    // comment out for now cuz we always them to pursue
    // if ((toEvader.dot(vehicle_.heading()) > 0) && (relativeHeading < -0.95))
    // {
    //     return seek(evader.position);
    // }

    float lookAheadTime = toEvader.length() / (vehicle_.maxSpeed + evader.speed());

    // the math below say where will evader be after lookAheadTime (secs)
    return seek(evader.position + evader.velocity * lookAheadTime);
}

sf::Vector2f SteeringBehaviors::evade(const Vehicle& pursuer) const
{
    sf::Vector2f toPursuer = pursuer.position - vehicle_.position;

    float lookAheadTime = toPursuer.length() / (vehicle_.maxSpeed + pursuer.speed());

    return flee(pursuer.position + pursuer.velocity * lookAheadTime);
}

sf::Vector2f SteeringBehaviors::wander()
{
    // adding small random displacement to the target
    wanderTarget += sf::Vector2f(randomClamped() * wanderJitter, randomClamped() * wanderJitter);

    if (wanderTarget.lengthSquared() > 1e-6f)
        wanderTarget = wanderTarget.normalized() * wanderRadius;

    sf::Vector2f targetLocal = wanderTarget + sf::Vector2f(wanderDistance, 0);
    sf::Vector2f targetWorld = pointToWorldSpace(targetLocal);
    return seek(targetWorld);
}

sf::Vector2f SteeringBehaviors::obstacleAvoidance(const std::vector<std::unique_ptr<Obstacle>>& obstacles)
{
    vehicle_.detection_box_length_ = vehicle_.min_detection_box_length_ + (vehicle_.speed() / vehicle_.maxSpeed) * vehicle_.min_detection_box_length_;

    tagObstaclesInRange(obstacles, vehicle_.detection_box_length_);

    Obstacle* closestIntersectingObstacle = nullptr;

    float distToClosestObstacle = std::numeric_limits<float>::max();

    sf::Vector2f localPosOfClosestObstacle;

    for (auto it = obstacles.begin(); it != obstacles.end(); ++it)
    {
        // if obstacle is tagged within range
        if ((*it)->isTagged())
        {
            sf::Vector2f localPos = pointToLocalSpace((*it)->getPosition());

            // ignore if behind agent
            if (localPos.x >= 0)
            {
                float expandedRadius = (*it)->getCollisionRadius() + vehicle_.collision_radius_;
                if (std::fabs(localPos.y) < expandedRadius)
                {
                    float circleX = localPos.x;
                    float circleY = localPos.y;

                    float sqrtPart = std::sqrt(expandedRadius * expandedRadius - circleY * circleY);

                    float ip = circleX - sqrtPart;

                    if (ip <= 0)
                    {
                        ip = circleX + sqrtPart;
                    }

                    // check if this is the closest intersecting obstacle so far
                    float expandedRadiusOfClosestObstacle = 0.0f;
                    if (ip < distToClosestObstacle)
                    {
                        distToClosestObstacle = ip;
                        closestIntersectingObstacle = it->get();
                        localPosOfClosestObstacle = localPos;
                        expandedRadiusOfClosestObstacle = expandedRadius;
                    }
                }
            }
        }
    }

    // no obstacle intersected
    if (closestIntersectingObstacle == nullptr)
    {
        return sf::Vector2f(0.0f, 0.0f);
    }

    sf::Vector2f steeringForce;

    float multiplier = 1.0 + (vehicle_.detection_box_length_- localPosOfClosestObstacle.x) / vehicle_.detection_box_length_;

    // calculate lateral force
    steeringForce.y = (closestIntersectingObstacle->getCollisionRadius() - localPosOfClosestObstacle.y) * multiplier;

    // braking force
    const float brakingWeight = 0.2;
    steeringForce.x = (closestIntersectingObstacle->getCollisionRadius() - localPosOfClosestObstacle.x) * brakingWeight;

    return vectorToWorldSpace(steeringForce);
}

sf::Vector2f SteeringBehaviors::pointToWorldSpace(sf::Vector2f targetLocal)
{
    sf::Vector2f heading = vehicle_.heading();
    sf::Vector2f side = vehicle_.side();
    sf::Vector2f position = vehicle_.position;

    return sf::Vector2f(
        position.x + heading.x * targetLocal.x + side.x * targetLocal.y,
        position.y + heading.y * targetLocal.x + side.y * targetLocal.y
    );
}

sf::Vector2f SteeringBehaviors::pointToLocalSpace(sf::Vector2f targetWorld)
{
    sf::Vector2f heading = vehicle_.heading();
    sf::Vector2f side = vehicle_.side();
    sf::Vector2f position = vehicle_.position;

    sf::Vector2f offset = targetWorld - position;

    float localX = offset.x * heading.x + offset.y * heading.y;
    float localY = offset.x * side.x + offset.y * side.y;

    return sf::Vector2f(localX, localY);
}

sf::Vector2f SteeringBehaviors::vectorToWorldSpace(sf::Vector2f vecLocal)
{
    sf::Vector2f heading = vehicle_.heading();
    sf::Vector2f side = vehicle_.side();

    return sf::Vector2f(
        heading.x * vecLocal.x + side.x * vecLocal.y,
        heading.y * vecLocal.x + side.y * vecLocal.y
    );
}

void SteeringBehaviors::tagObstaclesInRange(const std::vector<std::unique_ptr<Obstacle>>& obstacles, float range)
{
    float rangeSqrt = range * range;
    for (const auto& obstacle : obstacles)
    {
        obstacle->untag();

        // distance from vehicle to obstacle
        sf::Vector2f dist = obstacle->getPosition() - vehicle_.position;
        float distSqrt = dist.x * dist.x  + dist.y * dist.y;

        if (distSqrt < rangeSqrt)
        {
            obstacle->tag();
        }
    }
}
