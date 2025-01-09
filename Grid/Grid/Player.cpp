#include "Player.h"

void Player::Initialize(float xStartPos, float yStartPos)
{
  velocity = sf::Vector2f(0.f, 0.f);
  body.setSize(sf::Vector2f(width, height));
  body.setPosition(sf::Vector2f(xStartPos, yStartPos));
  body.setFillColor(sf::Color::Red);

  testRect.setSize(sf::Vector2f(300.f, 200.f));
  testRect.setPosition(sf::Vector2f(body.getPosition().x + 100.f, body.getPosition().y));
  testRect.setFillColor(sf::Color::White);

  contactPointVisual.setSize(sf::Vector2f(10.f, 10.f));
  contactPointVisual.setFillColor(sf::Color::Blue);
}

void Player::Load()
{
}

void Player::Update(float deltaTime, WorldGrid worldGrid)
{
  sf::Vector2f rayOrigin = sf::Vector2f(body.getPosition().x + body.getSize().x / 2, body.getPosition().y + body.getSize().y / 2);
  sf::Vector2f rayDirection = velocity * 20.f;

  sf::Vector2f contactPoint, contactNormal;
  float time;
  
  rayVisual[0].position = rayOrigin;
  rayVisual[0].color = sf::Color::Magenta;
  rayVisual[1].position = sf::Vector2f(body.getPosition().x + body.getSize().x / 2, body.getPosition().y + body.getSize().y / 2) + velocity ;
  rayVisual[1].color = sf::Color::Red;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
  {
    velocity.x = -moveSpeed;
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
  {
    velocity.x = moveSpeed;
  }
  else
  {
    velocity.x = 0.f;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
  {
    velocity.y = -moveSpeed;
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
  {
    velocity.y = moveSpeed;
  }
  else
  {
    velocity.y = 0.f;
  }
  

  if (IsColliding(body, testRect, contactPoint, contactNormal, time, deltaTime))
  {
    velocity += sf::Vector2f(contactNormal.x * std::abs(velocity.x), contactNormal.y * std::abs(velocity.y)) * (1 - time);

    testRect.setFillColor(sf::Color::Yellow);
    
  }
  else
  {
    testRect.setFillColor(sf::Color::White);
  }

  velocity *= deltaTime;
  body.move(velocity);
}

bool Player::IsColliding(const sf::RectangleShape& in, const sf::RectangleShape& target, sf::Vector2f& contactPoint, 
                         sf::Vector2f& contactNormal, float& contactTime, float fElapsedTime)
{
  if(velocity.x == 0 && velocity.y == 0) return false;

  sf::RectangleShape expandedTarget;
  expandedTarget.setPosition(target.getPosition().x - in.getSize().x / 2, target.getPosition().y - in.getSize().y / 2);
  expandedTarget.setSize(target.getSize() + in.getSize());

  if (RayTest(sf::Vector2f(in.getPosition().x + in.getSize().x / 2, in.getPosition().y + in.getSize().y / 2), velocity * fElapsedTime, expandedTarget, contactPoint, contactNormal, contactTime))
  {
    if (contactTime < 1.0f && contactTime >= 0.0f) return true;
  }

  return false;

}

bool Player::RayTest(const sf::Vector2f& rayOrigin, const sf::Vector2f& rayDir, const sf::RectangleShape& target, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& t_hit_near)
{
  float t_nearX = (target.getPosition().x - rayOrigin.x) / rayDir.x;
  float t_nearY = (target.getPosition().y - rayOrigin.y) / rayDir.y;
  sf::Vector2f t_near = sf::Vector2f(t_nearX, t_nearY);

  float t_farX = (target.getPosition().x + target.getSize().x - rayOrigin.x) / rayDir.x;
  float t_farY = (target.getPosition().y + target.getSize().y - rayOrigin.y) / rayDir.y;
  sf::Vector2f t_far = sf::Vector2f(t_farX, t_farY);

  if(t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
  if(t_near.y > t_far.y) std::swap(t_near.y, t_far.y);

  if(t_near.x > t_far.y || t_near.y > t_far.x) return false;

  t_hit_near = std::max(t_near.x, t_near.y);
  float t_hit_far = std::min(t_far.x, t_far.y);

  if (t_hit_far < 0) return false;

  contactPoint = rayOrigin + t_hit_near * rayDir;

  if (t_near.x > t_near.y)
  {
    if (rayDir.x < 0)
    {
      contactNormal = sf::Vector2f(1.f, 0);
    }
    else
    {
      contactNormal = sf::Vector2f(-1.f, 0);
    }
  }
  else if (t_near.x < t_near.y)
  {
    if (rayDir.y < 0)
    {
      contactNormal = sf::Vector2f(0, 1.f);
    }
    else
    {
      contactNormal = sf::Vector2f(0, -1.f);
    }
  }
  return true;
}

void Player::Draw()
{
}