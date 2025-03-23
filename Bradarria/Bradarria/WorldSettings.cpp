#include "WorldSettings.h"

void WorldSettings::Update(sf::RenderWindow& window, SceneManager& sceneManager)
{
  sf::Event event;
  while (window.pollEvent(event))
  {
    if (event.type == sf::Event::TextEntered)
    {
      if (std::isdigit(event.text.unicode) && playerInput.getSize() < 9)
      {
        playerInput += event.text.unicode;
        playerText.setString( "Seed: " + playerInput);
      }
    }
    if (event.type == sf::Event::KeyPressed)
    {
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace) && playerInput.getSize() > 0)
      {
        playerInput.erase(playerInput.getSize() - 1);
        playerText.setString("Seed: " + playerInput);
      }
      else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
      {
        sceneManager.currentScene = sceneManager.mainMenu;
      }
      else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
      {
        sceneManager.currentScene = sceneManager.game;
      }
    }
  }
}
