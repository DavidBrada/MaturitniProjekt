#include <SFML/Graphics.hpp>
#include <iostream>
#include "WorldGrid.h"
#include "TileSelector.h"
#include "UI.h"
#include "Player.h"
#include "Item.h"
#include "Settings.h"

int main()
{
  float dt = 0.f;
  sf::Clock dtClock;
  sf::Clock uiClock; // updates UI

  // Mouse position relative to the screen; used for debugging
  sf::Vector2i mousePosScreen = sf::Mouse::getPosition();

  sf::RenderWindow window(sf::VideoMode(1920, 1080), "Bradarria", sf::Style::Default);
  window.setFramerateLimit(60);
  window.setKeyRepeatEnabled(false);

  //Create game objects
  WorldGrid worldGrid;
  TileSelector tileSelector;
  Player player;
  UI ui;
  sf::View view;
  Item item;
  Settings settings;


  // Initialize game objects
  worldGrid.Initialize();
  tileSelector.Initialize(worldGrid);
  player.Initialize((worldGrid.mapWidth / 2) * worldGrid.tileSize, worldGrid.groundLevel * worldGrid.tileSize - 4 * worldGrid.tileSize, worldGrid);
  ui.Initialize();
  settings.Load();
  
  view.setSize(1920.f, 1080.f);
  view.setCenter(sf::Vector2f(player.body.getPosition().x + player.width / 2, player.body.getPosition().y + player.height / 2)); // Initializes player view in the middle of the whole tilemap

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
    player.Update(dt, worldGrid, view);

    // Update UI
    ui.Update(worldGrid, tileSelector, player);
    /*
    if (uiClock.getElapsedTime().asSeconds() >= ui.timeStep)
    {
      ui.Update(worldGrid, tileSelector, player);
      uiClock.restart();
    }
    */
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
            player.Jump();
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
          tileSelector.GetClickPos(worldGrid);
          tileSelector.mineClock.restart();
        }
        break;

      case sf::Event::MouseButtonReleased:
        tileSelector.mining = false;

      }
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
      if (tileSelector.selectedType != worldGrid.tileMap[tileSelector.selectorPosition.x / worldGrid.tileSize][tileSelector.selectorPosition.y / worldGrid.tileSize].type &&
          tileSelector.canPlace && tileSelector.selectedType != 0)
      {
        worldGrid.PlaceTile(tileSelector.selectedType, worldGrid.mousePosGrid.x, worldGrid.mousePosGrid.y);
      }
      if (tileSelector.selectedType == 0)
      {
        if (settings.instaBreak)
        {
          worldGrid.PlaceTile(0, worldGrid.mousePosGrid.x, worldGrid.mousePosGrid.y);
        }
        else if (tileSelector.clickPosition.x == worldGrid.mousePosGrid.x &&
                 tileSelector.clickPosition.y == worldGrid.mousePosGrid.y &&
                 worldGrid.tileMap[tileSelector.selectorPosition.x / worldGrid.tileSize][tileSelector.selectorPosition.y / worldGrid.tileSize].hasCollision)
        {
          tileSelector.mining = true;

          if (tileSelector.mineClock.getElapsedTime().asSeconds() >= 1.f)
          {
            worldGrid.PlaceTile(0, worldGrid.mousePosGrid.x, worldGrid.mousePosGrid.y);
          }
        }
        else
        {
          tileSelector.mineClock.restart();
          tileSelector.clickPosition.x = worldGrid.mousePosGrid.x;
          tileSelector.clickPosition.y = worldGrid.mousePosGrid.y;
          tileSelector.mining = false;
        }
      }

      
    }

#pragma region Rendering

    window.clear(sf::Color(135, 206, 235));

    // Render Game Objects
    window.setView(view);

    worldGrid.Render(window, view);
    window.draw(tileSelector.selectorBody);
    player.Draw(window, worldGrid);

    // Render UI
    window.setView(window.getDefaultView());
    ui.Render(window);

    window.display();
#pragma endregion

  }

  return 0;
}