#pragma once
#include "SFML/Graphics.hpp"
#include "Tile.h"
#include "WorldGrid.h"
#include "TileSelector.h"
#include "Settings.h"
#include "Inventory.h"

class WorldGrid;

class Player
{
public:
  int width = 25; // One tile is 16 wide, so it's a little more than one
  int height = 40; // One tile is 16 tall, so it's a little more than two
  sf::RectangleShape body; // Acts kinda like a hitbox visualisation
  sf::Sprite sprite; // Actual player texture drawn on screen
  sf::Texture texture;
  float gravity = 0.f; // Initialized here to zero, because  the player was falling through the map and now I don't wanna touch it cuz it would definitely break everyting
  bool colliding;
  bool bodyOutOfCamBounds; // Set to true when the player goes a certain distance from the view center

  int animFrameCount;
  int currentAnimFrame;
  sf::Clock animClock;
  int state;

  // Jumping
  bool grounded;
  bool jumping;
  float moveSpeed = 150.f;
  sf::Clock jumpCooldown;
  sf::Clock jumpTimer;

  // More stupid code
  float defaultJumpForce = 900.f; // Since jumpForce is devided by the fade value, this stores the original one
  float jumpForce;
  float jumpForceFade = 1.1f; // how quickly jumpforce loses its value (jumpForce is devided by this value)
  float jumpTime = 0.25f; // how long will jumpForce be applied in seconds

  bool canPlace; // If a block can be placed at current cursor position
  bool mining;
  int xCanPlaceFrom;
  int xCanPlaceTo;
  int yCanPlaceFrom;
  int yCanPlaceTo;

  sf::Vector2f velocity;
  sf::Vector2f inputVelocity; // Helps determine which movement key is pressed (basically a unit vector)

  int xViewMoveCenterOffset; // What distance from the center of the view does the player need to be in order for the view to start moving
  int yViewMoveCenterOffset;

  sf::RectangleShape groundCheckRectLeft; // When this object collides with the ground, the player is considered grounded
  sf::RectangleShape groundCheckRectRight; // Or this one
  sf::RectangleShape groundCheckRectCenter; // Or this one

  // Collision checking - only check tiles around the player
  int cFromX = 0;
  int cToX = 0;
  int cFromY = 0;
  int cToY = 0;

  void Update(float& deltaTime, WorldGrid& worldGrid, sf::View& view, sf::RectangleShape& tileSelectorBody);
  void Jump();
  bool RayTest(const sf::Vector2f& rayOrigin, const sf::Vector2f& rayDir, const sf::RectangleShape& target, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& t_hit_near);
  bool IsColliding(const sf::RectangleShape& in, const sf::RectangleShape& target, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& contactTime, float fElapsedTime);
  bool IsGrounded(const sf::RectangleShape& in, const sf::RectangleShape& target, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& contactTime, float fElapsedTime);
  bool InArea(sf::RectangleShape& other, int maxDistance); // If a player can perform an action based on the distance from an object
  void Draw(sf::RenderWindow& window, WorldGrid& worldGrid);

  enum States
  {
    idle,
    walking
  };

  Player(float xStartPos, float yStartPos)
  {
    gravity = 300.f;
    velocity = sf::Vector2f(0.f, 0.f);
    body.setSize(sf::Vector2f(width, height));
    body.setPosition(sf::Vector2f(xStartPos, yStartPos));
    body.setFillColor(sf::Color::Transparent);
    body.setOutlineColor(sf::Color::Green);
    body.setOutlineThickness(2);

    groundCheckRectLeft.setSize(sf::Vector2f(width / 8, width / 4));
    groundCheckRectLeft.setFillColor(sf::Color::Blue);

    groundCheckRectRight.setSize(sf::Vector2f(width / 8, width / 4));
    groundCheckRectRight.setFillColor(sf::Color::Blue);

    groundCheckRectCenter.setSize(sf::Vector2f(width / 8, width / 4));
    groundCheckRectCenter.setFillColor(sf::Color::Blue);

    jumpForce = defaultJumpForce;
    xViewMoveCenterOffset = 100.f;
    yViewMoveCenterOffset = 50.f;

    if (texture.loadFromFile("assets/textures/PlayerAnimTest.png"))
    {
      sprite.setTexture(texture);
      animFrameCount = texture.getSize().x / width;
      std::cout << "Successfully loaded player texture" << std::endl;
    }
    else
    {
      std::cout << "Failed to load player texture" << std::endl;
    }
    sprite.setTextureRect(sf::IntRect(0, 0, width, height)); // Start the animation on the first frame
  }

  ~Player();
};