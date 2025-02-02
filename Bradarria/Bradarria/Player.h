#pragma once
#include "SFML/Graphics.hpp"
#include "Tile.h"
#include "WorldGrid.h"

class Player
{
public:
  int width = 25; // one tile is 16 wide, so it's a little more than one
  int height = 40; //  one tile is 16 tall
  sf::RectangleShape body; // Player object that is displayed
  sf::Sprite sprite;
  sf::Texture texture;
  float gravity = 300.f;
  bool colliding;

  // Jumping
  bool grounded;
  bool jumping;
  float moveSpeed = 150.f;
  sf::Clock jumpCooldown;
  sf::Clock jumpTimer;
  float defaultJumpForce = 900.f; // Since jumpForce is devided by the fade value, this stores the original one
  float jumpForce;
  float jumpForceFade = 1.1f; // how quickly jumpforce loses its value (jumpForce is devided by this value)
  float jumpTime = 0.25f; // how long will jumpForce be applied in seconds

  sf::Vector2f velocity;
  sf::Vector2f inputVelocity; // Helps determine which movement key is pressed (basically a unit vector)


  sf::Vertex contactNormalLine[2]; // Draws the normal vector of a collision
  sf::RectangleShape collisionAreaVisual; // Used to visualize what tiles are checked for collision with the player object

  sf::RectangleShape groundCheckRectLeft; // When this object collides with the ground, the player is considered grounded
  sf::RectangleShape groundCheckRectRight; // Or this one

  // Collision checking - only check tiles around the player
  int cFromX = 0;
  int cToX = 0;
  int cFromY = 0;
  int cToY = 0;

  void Initialize(float xStartPos, float yStartPos, WorldGrid& worldGrid);
  void Load();
  void Update(float& deltaTime, WorldGrid& worldGrid, sf::View& view);
  void Jump();
  bool RayTest(const sf::Vector2f& rayOrigin, const sf::Vector2f& rayDir, const sf::RectangleShape& target, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& t_hit_near);
  bool IsColliding(const sf::RectangleShape& in, const sf::RectangleShape& target, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& contactTime, float fElapsedTime);
  bool IsGrounded(const sf::RectangleShape& in, const sf::RectangleShape& target, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& contactTime, float fElapsedTime);
  void Draw(sf::RenderWindow& window, WorldGrid& worldGrid);
};