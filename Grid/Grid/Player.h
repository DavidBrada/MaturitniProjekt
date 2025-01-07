#pragma once
#include "SFML/Graphics.hpp"
#include "Tile.h"
#include "WorldGrid.h"

class Player
{
  int width = 36;
  int height = 72;


public:
  float moveSpeed = 250.f;
  sf::Vector2f velocity;

  sf::RectangleShape body;
  sf::RectangleShape testRect;
  sf::RectangleShape rayRect;
  sf::RectangleShape contactPointVisual;
  sf::Vertex contactNormalLine[2];
  sf::Vertex rayVisual[2];

  void Initialize(float xStartPos, float yStartPos);
  void Load();
  void Update(float deltaTime, WorldGrid worldGrid);
  bool RayTest(const sf::Vector2f& rayOrigin, const sf::Vector2f& rayDir, const sf::RectangleShape& target, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& t_hit_near);
  bool IsColliding(const sf::RectangleShape& in, const sf::RectangleShape& target, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& contactTime, float fElapsedTime);
  void Draw();
};