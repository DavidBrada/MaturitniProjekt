#include "Player.h"
#include "GameLogic.h"

void Player::Initialize()
{

  body.setSize(sf::Vector2f(width, height));

  hitBox.width = body.getSize().x;
  hitBox.height = body.getSize().y;

  groundCheck.width = body.getSize().x;
  groundCheck.height = body.getSize().y;

  visualGroundCheck.setSize(sf::Vector2f(groundCheck.getSize().x, groundCheck.getSize().y));
  visualGroundCheck.setFillColor(sf::Color::Transparent);
  visualGroundCheck.setOutlineThickness(2);
  visualGroundCheck.setOutlineColor(sf::Color::Yellow);
}

void Player::Load(int xPosition, int yPosition)
{
  body.setPosition(sf::Vector2f(xPosition, yPosition));
  body.setOrigin(sf::Vector2f(body.getSize().x / 2, body.getSize().y / 2));

  body.setFillColor(sf::Color::Blue);
}

void Player::Update(sf::FloatRect& ground, double deltaTime)
{
  // Ground check
  if (!groundCheck.intersects(ground))
  {
    isGrounded = false;
    if (currentGravity < maxGravity)
    {
      currentGravity += gravityAcceleration;
    }
    else
    {
      currentGravity = maxGravity;
    }

    //std::cout << "adding player gravity!" << std::endl;
    yVel += currentGravity;
  }
  else
  {
    if (!isGrounded)
    {
      yVel = 0;
      currentGravity = 0;
      isGrounded = true;
      body.setPosition(body.getPosition().x, ground.getPosition().y - body.getSize().y / 2);
      //std::cout << "Collision!" << std::endl;
    }
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
  {
    xVel -= moveSpeed;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
  {
    xVel += moveSpeed;
  }

  body.move(xVel * deltaTime, yVel * deltaTime);

  groundCheck.top = body.getPosition().y;
  groundCheck.left = body.getPosition().x - body.getSize().x / 2;

  visualGroundCheck.setPosition(sf::Vector2f(groundCheck.left, groundCheck.top));
  xVel = 0;
}

void Player::Draw(sf::RenderWindow& window)
{
  window.draw(body);
  window.draw(visualGroundCheck);
}
