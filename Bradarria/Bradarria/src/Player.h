#pragma once
#include <SFML/Graphics.hpp>
class Player
{
private:
  const int width = 90;
  const int height = 180;

  const int gravityAcceleration = 25;
  const int maxGravity = 500;

public:
  sf::FloatRect groundCheck;
  sf::RectangleShape visualGroundCheck;
  sf::RectangleShape body;
  sf::FloatRect hitBox;

  float moveSpeed = 1000;
  bool isGrounded {};
  int currentGravity;
  float xVel{};
  float yVel{};

public:
  void Initialize();
  void Load(int xPosition, int yPosition);
  void Update(sf::FloatRect& ground, double deltaTime);
  void Draw(sf::RenderWindow& window);
};