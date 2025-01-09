#include <SFML/Graphics.hpp>
#include <iostream>
#include "WorldGrid.h"
#include "TileSelector.h"
#include "UI.h"
#include "Player.h"

int main()
{
  float dt = 0.f;
  sf::Clock dtClock;
  sf::Clock uiClock; // updates UI

  // Mouse position relative to the screen; used for debugging
  sf::Vector2i mousePosScreen = sf::Mouse::getPosition();

  sf::RenderWindow window(sf::VideoMode(1920, 1080), "TILEMAP", sf::Style::Fullscreen);
  window.setFramerateLimit(60);
  window.setKeyRepeatEnabled(false);

  //Create game objects
  WorldGrid worldGrid;
  TileSelector tileSelector;
  UI ui;
  Player player;
  sf::View view;

  view.setSize(1920.f, 1080.f);
  view.setCenter(sf::Vector2f(worldGrid.mapWidth / 2 * worldGrid.tileSize, worldGrid.mapHeight / 2 * worldGrid.tileSize)); // Initializes player view in the middle of the whole tilemap
  float viewSpeed = 400.f; // movement speed of the camera

  // Initialize game objects
  worldGrid.Initialize();
  tileSelector.Initialize(worldGrid);
  ui.Initialize();
  player.Initialize(view.getCenter().x, view.getCenter().y - 4 * worldGrid.tileSize, worldGrid);
  

  while (window.isOpen())
  {
    // Delta Time
    dt = dtClock.restart().asSeconds();

    mousePosScreen = sf::Mouse::getPosition();
    window.setView(view);

    worldGrid.Update(window);

    window.setView(window.getDefaultView());

    //Update Game
    tileSelector.Update(worldGrid);
    player.Update(dt, worldGrid);

    // Update UI
    if (uiClock.getElapsedTime().asSeconds() > ui.timeStep)
    {
      ui.Update(worldGrid, tileSelector, player);
      uiClock.restart();
    }

    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
      switch (event.type)
      {
      case sf::Event::Closed:
        window.close();
        break;

      case sf::Event::KeyPressed:

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
          if (player.grounded && !player.jumping)
          {
            player.jumpCooldown.restart();
            player.jumping = true;
            player.velocity.y = 0.f;

            //player.body.move(0.f, -player.jumpForce);
            //player.velocity.y -= sqrtf(2.0f * 981.f * player.jumpForce);
            player.velocity.y -= player.jumpForce;
          }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
        {
          if (ui.visible)
          {
            ui.visible = false;
          }
          else
          {
            ui.visible = true;
          }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
          window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
        {
          tileSelector.selectedType = 0;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
        {
          tileSelector.selectedType = 1;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
        {
          tileSelector.selectedType = 2;
        }
        break;

      case sf::Event::MouseButtonPressed:
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
          worldGrid.PlaceTile(tileSelector.selectedType, worldGrid.mousePosGrid.x, worldGrid.mousePosGrid.y);
        }
      }
    }


#pragma region Rendering

    window.clear();

    // Render Game Objects
    window.setView(view);

    worldGrid.Render(window, view);
    window.draw(tileSelector.selectorBody);
    player.Draw(window, worldGrid);
    
    window.draw(player.body);

    // Render UI
    window.setView(window.getDefaultView());
    ui.Render(window);

    window.display();
#pragma endregion

  }

  return 0;
}