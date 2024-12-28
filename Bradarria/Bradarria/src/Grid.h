#pragma once
#include <SFML/Graphics.hpp>
class Grid
{
  sf::RectangleShape line;
public:
  Grid();
  ~Grid();

  void Initialize();
  void Load();
  void Update(double deltaTime);
  void Draw(sf::RenderWindow& window);
};

