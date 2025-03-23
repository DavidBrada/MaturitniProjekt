#include "MainMenu.h"

void MainMenu::Update(sf::RenderWindow& window, SceneManager& sceneManager)
{
  mousePos = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
  sf::Event event;
  while (window.pollEvent(event))
  {
    switch (event.type)
    {
    case sf::Event::Closed:
      window.close();
      break;
    case sf::Event::KeyPressed:
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
      {
        sceneManager.currentScene = sceneManager.game;
      }
      break;
    }
  }

  if (playButtonHitBox.contains(mousePos) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
  {
    sceneManager.currentScene = sceneManager.game;
  }
}

void MainMenu::Render(sf::RenderWindow& window)
{
  window.draw(bgSprite);
  window.draw(playButton);
  //window.draw(exitButton);
}
