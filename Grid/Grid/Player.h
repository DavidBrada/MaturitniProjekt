#pragma once
#include "SFML/Graphics.hpp"
#include "Tile.h"
#include "WorldGrid.h"

class Player
{
  int width = 36;
  int height = 72;

  float downForce = 10.f;

public:
  float gravity;
  sf::Clock jumpCooldown;
  bool grounded;
  bool jumping;
  float moveSpeed = 250.f;
  float jumpForce = 1000.f;
  float maxGravity = 5000.f;
  sf::Vector2f velocity;

  sf::RectangleShape body;

  sf::RectangleShape collisionAreaVisual; // Used to visualize what tiles are checked for collision with the player object
  sf::RectangleShape groundCheckRect; // When this object collides with the ground, the player is considered grounded

  // Collision checking - only check tiles around the player
  int cFromX = 0;
  int cToX = 0;
  int cFromY = 0;
  int cToY = 0;

  void Initialize(float xStartPos, float yStartPos, WorldGrid& worldGrid);
  void Load();
  void Update(float& deltaTime, WorldGrid& worldGrid);
  bool RayTest(const sf::Vector2f& rayOrigin, const sf::Vector2f& rayDir, const sf::RectangleShape& target, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& t_hit_near);
  bool IsColliding(const sf::RectangleShape& in, const sf::RectangleShape& target, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& contactTime, float fElapsedTime);
  bool IsGrounded(const sf::RectangleShape& in, const sf::RectangleShape& target, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& contactTime, float fElapsedTime);
  void Draw(sf::RenderWindow& window, WorldGrid& worldGrid);
};