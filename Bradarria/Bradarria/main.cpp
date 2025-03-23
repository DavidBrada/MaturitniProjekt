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
#include "MainMenu.h"
#include "SceneManager.h"


void generateWorld(WorldGrid& worldGrid, int mapWidth, int mapHeight)
{
  worldGrid.Load();
}

int main()
{
  Settings settings;
  settings.Load();

  sf::RenderWindow window(sf::VideoMode(settings.windowWidth, settings.windowHeight), "Bradarria", sf::Style::Fullscreen);
  window.setFramerateLimit(settings.fpsLimit);
  window.setKeyRepeatEnabled(false);

  SceneManager sceneManager;
  MainMenu mainMenu(window);

  WorldGrid* worldGrid = nullptr;
  Player* player = nullptr;
  sf::View* view = nullptr;
  UI* ui = nullptr;
  Inventory* inventory = nullptr;
  TileSelector* tileSelector = nullptr;
  sf::Clock dtClock;

  float dt = 0.f;
  bool isLoaded = false;

  while (window.isOpen())
  {
    if (sceneManager.currentScene == sceneManager.mainMenu)
    {
      mainMenu.Update(window, sceneManager);

      window.clear(sf::Color(135, 206, 235));
      mainMenu.Render(window);
      window.display();

    }
    else if(sceneManager.currentScene == sceneManager.game)
    {
      if (!isLoaded)
      {
        LoadingScreen loadingScreen;
        
        // Display loading screen before generating the world
        window.clear(sf::Color(135, 206, 235));
        window.draw(loadingScreen.sprite);
        window.display();

        worldGrid = new WorldGrid();
        std::thread loadingThread(generateWorld, std::ref(*worldGrid), worldGrid->mapWidth, worldGrid->mapHeight);
        loadingThread.join(); // Wait for thread to finish

        tileSelector = new TileSelector();
        tileSelector->Initialize(*worldGrid);
        ui = new UI();
        ui->Initialize();
        player = new Player(worldGrid->playerSpawnPos.x, worldGrid->playerSpawnPos.y);

        inventory = new Inventory();
        inventory->Initialize();

        view = new sf::View();
        view->setSize(settings.windowWidth, settings.windowHeight);
        view->setCenter(sf::Vector2f(player->body.getPosition().x + player->width / 2, player->body.getPosition().y + player->height / 2)); // Initializes player view on player position

        inventory->Load();
        isLoaded = true;
      }

      sf::Vector2i mousePosScreen = sf::Mouse::getPosition();
      
      // Delta Time
      dt = dtClock.restart().asSeconds();

      mousePosScreen = sf::Mouse::getPosition();
      window.setView(*view);

      worldGrid->Update(window);

      window.setView(window.getDefaultView());

      //Update Game
      tileSelector->Update(*worldGrid);
      player->Update(dt, *worldGrid, *view, tileSelector->selectorBody);

      // Update UI
      ui->Update(*worldGrid, *tileSelector, *player, *inventory);
      inventory->Update(*worldGrid, window);

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
            if (player->grounded && !player->jumping)
            {
              player->Jump();
            }
          }
          else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
          {
            ui->visible = !ui->visible;
          }
          else if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
          {
            inventory->open = !inventory->open;
          }
          else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
          {
            // ADD - Delete game objects here and display main menu
            window.close();
          }
          else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
          {
            settings.instaBreak = !settings.instaBreak;
          }

          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
          {
            inventory->selectedPosition = sf::Vector2f(0, 0);
          }
          else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
          {
            inventory->selectedPosition = sf::Vector2f(1, 0);
          }
          else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
          {
            inventory->selectedPosition = sf::Vector2f(2, 0);
          }
          else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
          {
            inventory->selectedPosition = sf::Vector2f(3, 0);
          }
          else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
          {
            inventory->selectedPosition = sf::Vector2f(4, 0);
          }
          else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))
          {
            inventory->selectedPosition = sf::Vector2f(5, 0);
          }
          else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7))
          {
            inventory->selectedPosition = sf::Vector2f(6, 0);
          }
          else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8))
          {
            inventory->selectedPosition = sf::Vector2f(7, 0);
          }
          else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9))
          {
            inventory->selectedPosition = sf::Vector2f(8, 0);
          }

          tileSelector->selectedType = inventory->storedItems[inventory->container[inventory->selectedPosition.x][inventory->selectedPosition.y].id];
          break;

        case sf::Event::MouseButtonPressed:
          if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
          {
            tileSelector->GetClickPos(*worldGrid);
            tileSelector->mineClock.restart();

            if (inventory->inInventory)
            {
              inventory->GetClickPos();
              if (inventory->storedItems[inventory->container[inventory->selectedPosition.x][inventory->selectedPosition.y].id] != -1)
              {
                tileSelector->selectedType = inventory->storedItems[inventory->container[inventory->selectedPosition.x][inventory->selectedPosition.y].id];
              }
            }
          }
          break;

        case sf::Event::MouseButtonReleased:
          player->mining = false;
        }
      }

      // Mining and placing logic. This can't be put into a function. 
      // It's throwing a syntax error or says that the function doesn't take 3 arguments when it has 3 arguments for some reason
      if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
      {
        if (tileSelector->selectedType != worldGrid->tileMap[tileSelector->selectorPosition.x / worldGrid->tileSize][tileSelector->selectorPosition.y / worldGrid->tileSize].type &&
            player->canPlace && tileSelector->selectedType != 0 && inventory->container[inventory->selectedPosition.x][inventory->selectedPosition.y].quantity > 0)
        {
          worldGrid->PlaceTile(tileSelector->selectedType, worldGrid->mousePosGrid.x, worldGrid->mousePosGrid.y, worldGrid->tileMap);
          inventory->container[inventory->selectedPosition.x][inventory->selectedPosition.y].quantity--;
        }
        if (worldGrid->tileMap[tileSelector->selectorPosition.x / worldGrid->tileSize][tileSelector->selectorPosition.y / worldGrid->tileSize].mineable &&
            tileSelector->selectedType == 0)
        {
          player->mining = true;
          if (settings.instaBreak)
          {
            tileSelector->timeToMine = 0.f;

            if (tileSelector->mineClock.getElapsedTime().asSeconds() >= tileSelector->timeToMine && !inventory->inInventory)
            {
              tileSelector->minedType = worldGrid->tileMap[tileSelector->selectorPosition.x / worldGrid->tileSize][tileSelector->selectorPosition.y / worldGrid->tileSize].type;

              if (worldGrid->mousePosGrid.y > worldGrid->terrainHeightValues[worldGrid->mousePosGrid.x])
              {
                worldGrid->PlaceTile(worldGrid->dirtBackground, worldGrid->mousePosGrid.x, worldGrid->mousePosGrid.y, worldGrid->tileMap);
              }
              else
              {
                worldGrid->PlaceTile(worldGrid->air, worldGrid->mousePosGrid.x, worldGrid->mousePosGrid.y, worldGrid->tileMap);
              }

#pragma region StoreItem
              for (int i = 0; i < inventory->inventorySize; i++)
              {
                if (inventory->storedItems[i] == tileSelector->minedType)
                {
                  for (int x = 0; x < inventory->xCellCount; x++)
                  {
                    for (int y = 0; y < inventory->yCellCount; y++)
                    {
                      if (inventory->container[x][y].id == i)
                      {
                        inventory->container[x][y].quantity++;
                      }
                    }
                  }

                  break;
                }
                else if (inventory->storedItems[i] == 0)
                {
                  inventory->storedItems[i] = tileSelector->minedType;

                  for (int x = 0; x < inventory->xCellCount; x++)
                  {
                    for (int y = 0; y < inventory->yCellCount; y++)
                    {
                      if (inventory->container[x][y].id == i)
                      {
                        inventory->SetSprite(tileSelector->minedType, x, y, worldGrid->tileAtlasTexture, worldGrid->atlasTiles);
                        inventory->container[x][y].quantity++;
                      }
                    }
                  }
                  break;
                }
              }
            }


#pragma endregion

          }
          else if (tileSelector->clickPosition.x == worldGrid->mousePosGrid.x &&
                   tileSelector->clickPosition.y == worldGrid->mousePosGrid.y &&
                   worldGrid->tileMap[tileSelector->selectorPosition.x / worldGrid->tileSize][tileSelector->selectorPosition.y / worldGrid->tileSize].mineable)
          {
            tileSelector->timeToMine = 0.5f;

            if (tileSelector->mineClock.getElapsedTime().asSeconds() >= tileSelector->timeToMine)
            {
              tileSelector->minedType = worldGrid->tileMap[tileSelector->selectorPosition.x / worldGrid->tileSize][tileSelector->selectorPosition.y / worldGrid->tileSize].type;

              if (worldGrid->mousePosGrid.y > worldGrid->terrainHeightValues[worldGrid->mousePosGrid.x])
              {
                worldGrid->PlaceTile(worldGrid->dirtBackground, worldGrid->mousePosGrid.x, worldGrid->mousePosGrid.y, worldGrid->tileMap);
              }
              else
              {
                worldGrid->PlaceTile(worldGrid->air, worldGrid->mousePosGrid.x, worldGrid->mousePosGrid.y, worldGrid->tileMap);
              }
#pragma region StoreItem
              for (int i = 0; i < inventory->inventorySize; i++)
              {
                if (inventory->storedItems[i] == tileSelector->minedType)
                {
                  for (int x = 0; x < inventory->xCellCount; x++)
                  {
                    for (int y = 0; y < inventory->yCellCount; y++)
                    {
                      if (inventory->container[x][y].id == i)
                      {
                        inventory->container[x][y].quantity++;
                      }
                    }
                  }

                  break;
                }
                else if (inventory->storedItems[i] == 0)
                {
                  inventory->storedItems[i] = tileSelector->minedType;

                  for (int x = 0; x < inventory->xCellCount; x++)
                  {
                    for (int y = 0; y < inventory->yCellCount; y++)
                    {
                      if (inventory->container[x][y].id == i)
                      {
                        inventory->SetSprite(tileSelector->minedType, x, y, worldGrid->tileAtlasTexture, worldGrid->atlasTiles);
                        inventory->container[x][y].quantity++;
                      }
                    }
                  }
                  break;
                }
              }
            }
          }
          else
          {
            tileSelector->mineClock.restart();
            tileSelector->clickPosition.x = worldGrid->mousePosGrid.x;
            tileSelector->clickPosition.y = worldGrid->mousePosGrid.y;
            player->mining = false;
          }
        }
      }

#pragma region Rendering

      window.clear(sf::Color(135, 206, 235));

      // Render Game Objects
      window.setView(*view);

      worldGrid->Render(window, *view);
      window.draw(tileSelector->selectorBody);
      player->Draw(window, *worldGrid);

      // Render UI
      window.setView(window.getDefaultView());
      inventory->Render(window);
      ui->UpdateInventory(*inventory, window);
      ui->Render(window, *inventory);

      window.display();
    
#pragma endregion
    }
  }
  

  delete player;
  delete worldGrid;
  delete view;
  delete ui;
  delete inventory;
  delete tileSelector;

  return 0;
}