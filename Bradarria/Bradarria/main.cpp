#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include "WorldGrid.h"
#include "TileSelector.h"
#include "UI.h"
#include "Player.h"
#include "Settings.h"
#include "FastNoiseLite.h"
#include "LoadingScreen.h"
#include "Inventory.h"

void generateWorld(WorldGrid& worldGrid, int mapWidth, int mapHeight)
{
  worldGrid.Load();
}

int main()
{
  Settings settings;
  settings.Load();

  sf::RenderWindow window(sf::VideoMode(settings.windowWidth, settings.windowHeight), "Bradarria", sf::Style::Default);
  window.setFramerateLimit(settings.fpsLimit);
  window.setKeyRepeatEnabled(false);

  LoadingScreen loadingScreen;
  loadingScreen.Initialize();

  // Display loading screen before generating the world
  window.clear();
  window.draw(loadingScreen.sprite);
  window.display();

  WorldGrid worldGrid;
  worldGrid.Initialize();

  std::thread loadingThread(generateWorld, std::ref(worldGrid), worldGrid.mapWidth, worldGrid.mapHeight);
  loadingThread.join(); // Wait for thread to finish

  sf::View view;
  Player player;
  UI ui;
  Inventory inventory;

  float dt = 0.f;
  sf::Clock dtClock;
  sf::Clock uiClock; // updates UI

  // Mouse position relative to the screen; used for debugging
  sf::Vector2i mousePosScreen = sf::Mouse::getPosition();
  
  TileSelector tileSelector;
  
  tileSelector.Initialize(worldGrid);
  ui.Initialize();
  player.Initialize(worldGrid.playerSpawnPos.x, worldGrid.playerSpawnPos.y, worldGrid);
  inventory.Inicialize();
  
  view.setSize(settings.windowWidth, settings.windowHeight);
  view.setCenter(sf::Vector2f(player.body.getPosition().x + player.width / 2, player.body.getPosition().y + player.height / 2)); // Initializes player view on player position

  inventory.Load(view);

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
    player.Update(dt, worldGrid, view, tileSelector.selectorBody, inventory);

    // Update UI
    ui.Update(worldGrid, tileSelector, player, inventory);
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
          ui.visible = !ui.visible;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
        {
          inventory.open = !inventory.open;
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
        player.mining = false;

      }
    }

    inventory.Update(view, worldGrid, window);

    // Mining and placing logic
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
      if (tileSelector.selectedType != worldGrid.tileMap[tileSelector.selectorPosition.x / worldGrid.tileSize][tileSelector.selectorPosition.y / worldGrid.tileSize].type &&
          player.canPlace && tileSelector.selectedType != 0)
      {
        worldGrid.PlaceTile(tileSelector.selectedType, worldGrid.mousePosGrid.x, worldGrid.mousePosGrid.y, worldGrid.tileMap);
      }
      if (worldGrid.tileMap[tileSelector.selectorPosition.x / worldGrid.tileSize][tileSelector.selectorPosition.y / worldGrid.tileSize].mineable &&
          tileSelector.selectedType == 0)
      {
        if (settings.instaBreak)
        {
          tileSelector.minedType = worldGrid.tileMap[tileSelector.selectorPosition.x / worldGrid.tileSize][tileSelector.selectorPosition.y / worldGrid.tileSize].type;

          if (worldGrid.mousePosGrid.y > worldGrid.terrainHeightValues[worldGrid.mousePosGrid.x])
          {
            worldGrid.PlaceTile(worldGrid.dirtBackground, worldGrid.mousePosGrid.x, worldGrid.mousePosGrid.y, worldGrid.tileMap);
          }
          else
          {
            worldGrid.PlaceTile(worldGrid.air, worldGrid.mousePosGrid.x, worldGrid.mousePosGrid.y, worldGrid.tileMap);
          }

        }
        else if (tileSelector.clickPosition.x == worldGrid.mousePosGrid.x &&
                 tileSelector.clickPosition.y == worldGrid.mousePosGrid.y &&
                 worldGrid.tileMap[tileSelector.selectorPosition.x / worldGrid.tileSize][tileSelector.selectorPosition.y / worldGrid.tileSize].mineable)
        {
          player.mining = true;

          if (tileSelector.mineClock.getElapsedTime().asSeconds() >= 1.f)
          {
            worldGrid.PlaceTile(worldGrid.air, worldGrid.mousePosGrid.x, worldGrid.mousePosGrid.y, worldGrid.tileMap);
          }
        }
        else
        {
          tileSelector.mineClock.restart();
          tileSelector.clickPosition.x = worldGrid.mousePosGrid.x;
          tileSelector.clickPosition.y = worldGrid.mousePosGrid.y;
          player.mining = false;
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
    inventory.Render(window);

    window.display();
#pragma endregion

  }

  return 0;
}