#pragma once
#include <SFML/Graphics.hpp>
class Player
{
private:
  const int width = 60;
  const int height = 180;

  const float gravityAcceleration = 0.05f;
  const int maxGravity = 2;

public:
  sf::FloatRect groundCheck;
  sf::RectangleShape visualGroundCheck;
  sf::RectangleShape body;
  sf::FloatRect hitBox;

  float moveSpeed = 1;
  bool isGrounded {};
  float currentGravity;
  float xVel;
  float yVel;

public:
  void Initialize();
  void Load(int xPosition, int yPosition);
  void Update(sf::FloatRect& ground, float deltaTime);
  void Draw(sf::RenderWindow& window);
};