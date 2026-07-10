//
// Created by hoang on 5/8/2026.
//

#include "Game.h"

#include "SFML/Graphics/CircleShape.hpp"

Game::Game()
    : window(sf::VideoMode(sf::Vector2u(3072, 1920)), "Steering Behaviors")
    , vehicle1(sf::Vector2f(300, 540))
    , vehicle2(sf::Vector2f(1800, 100))
{
    vehicle1.maxSpeed = 300;
    vehicle2.maxSpeed = 200;

    spawnObstacles(3);
}

void Game::run()
{
    while (window.isOpen())
    {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents()
{
    while ( const std::optional event = window.pollEvent() )
    {
        if (event->is<sf::Event::Closed>())
            window.close();

        if (const auto* resizedEvent = event->getIf<sf::Event::Resized>())
        {
            sf::FloatRect visibleArea(sf::Vector2f(0, 0), sf::Vector2f(resizedEvent->size.x, resizedEvent->size.y));
            window.setView(sf::View(visibleArea));
        }
    }
}

void Game::update()
{
    float dt = clock.restart().asSeconds();
    sf::Vector2f target = sf::Vector2f(sf::Mouse::getPosition(window));

    //sf::Vector2f steeringForceV1 = vehicle1.steeringBehaviors.wander(dt);
    sf::Vector2f steeringForceV1 = sf::Vector2f(100,0);

    sf::Vector2f steeringForceV2 = vehicle2.steeringBehaviors.wander();
    //sf::Vector2f steeringForceV2 = vehicle2.steeringBehaviors.seek(vehicle1.position);

    vehicle1.update(dt, steeringForceV1, window.getSize());
    vehicle2.update(dt, steeringForceV2, window.getSize());
}

void Game::render()
{
    window.clear(sf::Color::Black);
    vehicle1.render(window);
    vehicle2.render(window);

    for (auto obstacle : obstacles)
    {
        obstacle.render(window);
    }

    window.display();
}

void Game::spawnObstacles(int amount)
{
    for (int i = 0; i < amount; i++)
    {
        Obstacle obstacle(randomInRange(50.0f, 200.0f));
        float posX = randomInRange(0, 1920);
        float posY = randomInRange(0, 1080);
        obstacle.setPosition(sf::Vector2f(posX, posY));
        obstacles.push_back(obstacle);
    }
}
