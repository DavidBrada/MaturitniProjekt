#include <SFML/Graphics.hpp>
#include <iostream>

#include "GameLogic.h"
#include "Player.h"
#include "Tile.h"
#include "World.h"
#include "Grid.h"

int main()
{

  GameLogic gameLogic;
  gameLogic.InitializeSettings();

  sf::RenderWindow window(sf::VideoMode(gameLogic.windowWidth, gameLogic.windowHeight), "Game window", sf::Style::Default, gameLogic.settings);
  window.setFramerateLimit(60);
  window.setKeyRepeatEnabled(false);

  //----------------------TEMPORARY TEST TILE(S)-------------------------
  std::vector<Tile> tileList;
  Tile placeholderTile;
  placeholderTile.Initialize();
  placeholderTile.body.setFillColor(sf::Color::Transparent);
  placeholderTile.body.setOutlineThickness(5);
  placeholderTile.body.setOutlineColor(sf::Color::Magenta);

  Tile placedTile;
  //-----------------------------------------------

  // --------------------Tilesheet Test-------------------------------
  World world;
  world.Initialize("assets/world_saves/world_1.world");
  //-------------------------------------------------------

  // Camera setup
  sf::View view(sf::Vector2f(0, 0), sf::Vector2f(gameLogic.xDefaultZoom, gameLogic.yDefaultZoom));

  // Ground placeholder object
  sf::RectangleShape ground(sf::Vector2f(2000.f, 200.f));
  ground.setPosition(-150, 400);
  sf::FloatRect groundBoundingBox = ground.getGlobalBounds();
  ground.setFillColor(sf::Color(0, 200, 0));

  //--------------GRID--------------------------
  Grid grid;
  grid.Initialize();
  //--------------------------------------------

  // Player object
  Player player;
  player.Initialize();
  player.Load(gameLogic.windowWidth / 2, 200); // Takes x, y spawn positions as arguments

  float groundTop = ground.getPosition().y;

  sf::Clock deltaTimeClock;

  // Game loop
  while (window.isOpen())
  {
    sf::Time deltaTimer = deltaTimeClock.restart();
    gameLogic.deltaTime = deltaTimer.asSeconds();


    sf::Event event;
    while (window.pollEvent(event))
    {
      switch (event.type)
      {
      case sf::Event::Closed:
        window.close();
        break;

      case sf::Event::KeyPressed:
        if (event.key.scancode == sf::Keyboard::Scan::Space && player.isGrounded)
        {
          player.yVel = 0;
          player.yVel = -2000;
          std::cout << "Jump!" << std::endl;

          /*
          //--------- JUST FOR DEBUGGING, DELETE LATER ----------------
          for (Tile tile : tileList)
          {
            std::cout << "x: " << tile.body.getPosition().x << std::endl << "y: " << tile.body.getPosition().y << std::endl;
          }
          //-----------------------------------------------------------
          */
        }

        // Block selecting and overriding section will be here

        if (event.key.scancode == sf::Keyboard::Scan::Num1)
        {
          placeholderTile.body.setOutlineColor(sf::Color::Cyan);
        }
        break;
       
      // Camera zoom on wheel scroll (up - closer ; down - further)
      case sf::Event::MouseWheelScrolled:
        std::cout << "Wheel delta: " << event.mouseWheelScroll.delta << std::endl;
        if (event.mouseWheelScroll.delta < 0 && gameLogic.zoomLevel < gameLogic.maxZoomLevel)
        {
          view.zoom(2);
          gameLogic.zoomLevel++;
        }
        else if(event.mouseWheelScroll.delta > 0 && gameLogic.zoomLevel > gameLogic.minZoomLevel)
        {
          view.zoom(0.5);
          gameLogic.zoomLevel--;
        }
        break;
      }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
      placedTile.Place(placeholderTile.position);
      tileList.push_back(placedTile);
    }

    // Debug logging
    //std::cout << "Velocity: " << player.xVel << std::endl;
    //std::cout << "gravity: " << player.currentGravity << std::endl;
    //std::cout << "grounded: " << isGrounded << std::endl;
    //std::cout << view.getSize().x << std::endl
    //std::cout << sf::Mouse::getPosition().x << std::endl;
    //std::cout << "Delta time: " << gameLogic.deltaTime << std::endl;

    sf::Vector2f mouseCoord = window.mapPixelToCoords(gameLogic.mousePosition);

    placeholderTile.Update(window, view, gameLogic.mousePosition);
    world.Update(gameLogic.deltaTime);
    player.Update(groundBoundingBox, gameLogic.deltaTime);

    view.setCenter(sf::Vector2f(player.body.getPosition().x, (window.getSize().y / 2) - 200));

    window.clear();
    window.setView(view);

    // Draw stuff here
    world.Draw(window);
    window.draw(ground);

    for (int i = 0; i < tileList.size(); i++)
    {
      window.draw(tileList[i].body);
    }

    player.Draw(window);
    window.draw(placeholderTile.body);
    grid.Draw(window);

    window.display();
  }

  return 0;
}