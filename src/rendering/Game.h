//
// Created by hoang on 5/8/2026.
//

#ifndef STEERINGBEHAVIORS_GAME_H
#define STEERINGBEHAVIORS_GAME_H

#include "SFML/Graphics/RenderWindow.hpp"
#include "../core/Vehicle.h"
#include "../core/Obstacle.h"
#include "../core/Utils.h"
#include "../core/Wall.h"


class Game
{
public:
    Game();
    void run();

private:
    void processEvents();
    void update();
    void render();

    void spawnObstacles(int amount);

    sf::RenderWindow window;
    sf::Clock clock;
    Vehicle vehicle1;
    Vehicle vehicle2;
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    std::unique_ptr<Wall> wall = std::make_unique<Wall>(sf::Vector2f(100.0f, 100.0f), sf::Vector2f(500.0f, 500.0f));
};

#endif //STEERINGBEHAVIORS_GAME_H
