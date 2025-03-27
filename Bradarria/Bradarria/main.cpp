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
#include "WorldSettings.h"
#include "SceneManager.h"
#include "CraftingMenu.h"


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

  SceneManager sceneManager;
  MainMenu mainMenu(window);
  WorldSettings worldSettings(window);

  WorldGrid* worldGrid = nullptr;
  Player* player = nullptr;
  sf::View* view = nullptr;
  UI* ui = nullptr;
  Inventory* inventory = nullptr;
  TileSelector* tileSelector = nullptr;
  CraftingMenu* craftingMenu = nullptr;
  sf::Clock dtClock;

  float dt = 0.f;
  bool isLoaded = false;
  bool exitGame = false;

  while (window.isOpen())
  {
    if (sceneManager.currentScene == sceneManager.mainMenu)
    {

      mainMenu.Update(window, sceneManager);

      window.clear(sf::Color(135, 206, 235));
      mainMenu.Render(window);
      window.display();

    }
    else if (sceneManager.currentScene == sceneManager.worldSettingsMenu)
    {
      worldSettings.Update(window, sceneManager);

      window.clear(sf::Color(135, 206, 235));
      worldSettings.Rneder(window);
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

        worldGrid->worldSeed = std::stoi(worldSettings.playerInput.toAnsiString());
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

        craftingMenu = new CraftingMenu();
        craftingMenu->Load(worldGrid);

        
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
      ui->Update(*worldGrid, *tileSelector, *player, *inventory, *craftingMenu);
      inventory->Update(*worldGrid, window);
      craftingMenu->Update(worldGrid, inventory);
#pragma region InputHandling



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
            exitGame = true;
            sceneManager.currentScene = sceneManager.mainMenu;
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
            else if (craftingMenu->inCrafting)
            {
              craftingMenu->GetClickPos();
            }
          }
          break;

        case sf::Event::MouseButtonReleased:
          player->mining = false;
        }
      }
#pragma endregion

      if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
      {
        if (tileSelector->selectedType != worldGrid->tileMap[tileSelector->selectorPosition.x / worldGrid->tileSize][tileSelector->selectorPosition.y / worldGrid->tileSize].type &&
            player->canPlace && tileSelector->selectedType != 0 && inventory->container[inventory->selectedPosition.x][inventory->selectedPosition.y].quantity > 0)
        {
          worldGrid->PlaceTile(tileSelector->selectedType, worldGrid->mousePosGrid.x, worldGrid->mousePosGrid.y, worldGrid->tileMap);
          inventory->container[inventory->selectedPosition.x][inventory->selectedPosition.y].quantity--;
        }

        if (worldGrid->tileMap[tileSelector->selectorPosition.x / worldGrid->tileSize][tileSelector->selectorPosition.y / worldGrid->tileSize].mineable &&
            tileSelector->selectedType == 0 && !inventory->inInventory && !craftingMenu->inCrafting)
        {
          worldGrid->MineTile(player, settings, tileSelector, inventory, craftingMenu);
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
      craftingMenu->Render(window);
      ui->UpdateInventory(*inventory, window);
      ui->Render(window, *inventory);

      window.display();
    
#pragma endregion

      if (exitGame)
      {
        exitGame = false;
        player->~Player();
        worldGrid->~WorldGrid();
        view->~View();
        ui->~UI();
        inventory->~Inventory();
        tileSelector->~TileSelector();
        craftingMenu->~CraftingMenu();
        isLoaded = false;
      }
    }
  }

  return 0;
}