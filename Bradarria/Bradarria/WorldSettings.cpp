#include "WorldSettings.h"

void WorldSettings::Update(sf::RenderWindow& window, SceneManager& sceneManager)
{
  mousePos = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
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
        if (playerInput.getSize() == 0)
        {
          playerInput = "0";
        }
        sceneManager.currentScene = sceneManager.game;
      }
    }
  }

  if (startButtonHitBox.contains(mousePos))
  {
    if (animationClock.getElapsedTime().asSeconds() >= 0.01f && startButtonText.getScale().x < 1.2f)
    {
      startButtonText.setScale(startButtonText.getScale().x + transformStep, startButtonText.getScale().y + transformStep);
      if (startButtonText.getOutlineThickness() < 6.f)
      {
        startButtonText.setOutlineThickness(startButtonText.getOutlineThickness() + 1.f);
      }
      animationClock.restart();
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
      if (playerInput.getSize() == 0)
      {
        playerInput = "0";
      }
      sceneManager.currentScene = sceneManager.game;
    }
  }
  else
  {
    if (animationClock.getElapsedTime().asSeconds() >= 0.01f && startButtonText.getScale().x > 1.f)
    {
      startButtonText.setScale(startButtonText.getScale().x - transformStep, startButtonText.getScale().y - transformStep);
      if (startButtonText.getOutlineThickness() > 0.f)
      {
        startButtonText.setOutlineThickness(startButtonText.getOutlineThickness() - 1.f);
      }
      animationClock.restart();
    }
  }
}

void WorldSettings::Rneder(sf::RenderWindow& window)
{
  window.draw(bgSprite);
  window.draw(playerText);
  //window.draw(startButton);
  window.draw(startButtonText);
}
