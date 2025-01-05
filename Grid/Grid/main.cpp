#include <SFML/Graphics.hpp>
#include <iostream>
#include "WorldGrid.h"
#include "TileSelector.h"
#include "UI.h"

int main()
{
  float dt = 0.f;
  sf::Clock dtClock;

  // Mouse position relative to the screen; used for debugging
  sf::Vector2i mousePosScreen = sf::Mouse::getPosition();

  sf::RenderWindow window(sf::VideoMode(1920, 1080), "GRID");
  window.setFramerateLimit(60);
  window.setKeyRepeatEnabled(false);

  //Create game objects
  WorldGrid worldGrid;
  TileSelector tileSelector;
  UI ui;

  sf::View view;

  //Initialize game objects
  worldGrid.Initialize();
  tileSelector.Initialize(worldGrid);
  ui.Initialize();
  
  view.setSize(1920.f, 1080.f);
  view.setCenter(sf::Vector2f(worldGrid.mapWidth / 2 * worldGrid.tileSize, worldGrid.mapHeight / 2 * worldGrid.tileSize)); // Initializes player view in the middle of the whole tilemap
  float viewSpeed = 200.f; // movement speed of the camera
  

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

    // Update UI
    ui.Update(worldGrid, tileSelector);

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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
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
          worldGrid.ChangeTile(tileSelector.selectedType);
        }
      }
    }

    // Get Input
#pragma region MovementInput

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
      view.move(-viewSpeed * dt, 0.f);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
      view.move(viewSpeed * dt, 0.f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
      view.move(0.f, -viewSpeed * dt);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
      view.move(0.f, viewSpeed * dt);
    }
#pragma endregion


#pragma region Rendering

    window.clear();

    // Render Game Objects
    window.setView(view);

    worldGrid.Render(window, view);
    window.draw(tileSelector.selectorBody);

    // Render UI
    window.setView(window.getDefaultView());
    ui.Render(window);

    window.display();
#pragma endregion

  }

  return 0;
}