#include "Player.h"

void Player::Initialize(float xStartPos, float yStartPos, WorldGrid& worldGrid)
{
  velocity = sf::Vector2f(0.f, 0.f);
  body.setSize(sf::Vector2f(width, height));
  body.setPosition(sf::Vector2f(xStartPos, yStartPos));
  body.setFillColor(sf::Color::Red);

  collisionAreaVisual.setSize(sf::Vector2f(worldGrid.tileSize * 3, worldGrid.tileSize * 4));
  collisionAreaVisual.setFillColor(sf::Color::Blue);

  groundCheckRect.setSize(sf::Vector2f(width - width / 2, width / 2));
  groundCheckRect.setFillColor(sf::Color::Blue);

  gravity = 100.f;
}

void Player::Load()
{
}

void Player::Update(float& deltaTime, WorldGrid& worldGrid)
{
  sf::Vector2f rayOrigin = sf::Vector2f(body.getPosition().x + body.getSize().x / 2, body.getPosition().y + body.getSize().y / 2);
  sf::Vector2f rayDirection = velocity * 20.f;

  
  collisionAreaVisual.setPosition(sf::Vector2f(body.getPosition().x - worldGrid.tileSize, body.getPosition().y - worldGrid.tileSize));

  sf::Vector2f contactPoint, contactNormal;
  float time;

  std::vector<std::pair<sf::Vector2i, float>> z;

#pragma region getInput
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
#pragma endregion
  // Gravity
  if (velocity.y < maxGravity)
  {
    velocity.y += gravity;
  }
  grounded = false;

  cFromX = body.getPosition().x / worldGrid.tileSize - 3;
  cToX = body.getPosition().x / worldGrid.tileSize + 4;
  cFromY = body.getPosition().y / worldGrid.tileSize - 1;
  cToY = body.getPosition().y / worldGrid.tileSize + 5;

  for (int x = cFromX; x < cToX; x++)
  {
    for (int y = cFromY; y < cToY; y++)
    {
      if (worldGrid.tileMap[x][y].hasCollision)
      {
        if (IsGrounded(groundCheckRect, worldGrid.tileMap[x][y].shape, contactPoint, contactNormal, time, deltaTime))
        {
          if (!grounded)
          {
            if (!jumping)
            {
              grounded = true;
            }

            if (jumpCooldown.getElapsedTime().asSeconds() >= 0.25f)
            {
              jumping = false;
              velocity.y = gravity;
            }
          }
        }

        if (IsColliding(body, worldGrid.tileMap[x][y].shape, contactPoint, contactNormal, time, deltaTime))
        {
          z.push_back({ sf::Vector2i(x, y), time });
        }
      }
    }
  }

  std::sort(z.begin(), z.end(), [](const std::pair<sf::Vector2i, float>& a, const std::pair<sf::Vector2i, float>& b)
            {
              return a.second < b.second;
            });

  for (auto j : z)
  {
    if (IsColliding(body, worldGrid.tileMap[j.first.x][j.first.y].shape, contactPoint, contactNormal, time, deltaTime))
    {
      velocity += sf::Vector2f(contactNormal.x * std::abs(velocity.x), contactNormal.y * std::abs(velocity.y)) * (1 - time);
    }
  }

  velocity *= deltaTime;
  body.move(velocity);
  groundCheckRect.setPosition(sf::Vector2f(body.getPosition().x + width / 4, body.getPosition().y + height));
}

bool Player::IsGrounded(const sf::RectangleShape& in, const sf::RectangleShape& target, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& contactTime, float fElapsedTime)
{

  if (RayTest(sf::Vector2f(in.getPosition().x + in.getSize().x / 2, in.getPosition().y + in.getSize().y / 2), sf::Vector2f(0.f, moveSpeed) * fElapsedTime, target, contactPoint, contactNormal, contactTime))
  {
    if (contactTime < 1.0f) return true;
  }
  return false;
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

void Player::Draw(sf::RenderWindow& window, WorldGrid& worldGrid)
{
  cFromX = body.getPosition().x / worldGrid.tileSize - 3;
  cToX = body.getPosition().x / worldGrid.tileSize + 4;
  cFromY = body.getPosition().y / worldGrid.tileSize - 2;
  cToY = body.getPosition().y / worldGrid.tileSize + 5;
  /*
  for (int x = cFromX; x < cToX; x++)
  {
    for (int y = cFromY; y < cToY; y++)
    {
      if (worldGrid.tileMap[x][y].shape.getGlobalBounds().intersects(collisionAreaVisual.getGlobalBounds()))
      {
        worldGrid.tileMap[x][y].shape.setOutlineColor(sf::Color::Magenta);
        worldGrid.tileMap[x][y].shape.setOutlineThickness(3.f);
      }
      else
      {
        worldGrid.tileMap[x][y].shape.setOutlineColor(sf::Color(150, 150, 150));
        worldGrid.tileMap[x][y].shape.setOutlineThickness(1.5f);
      }
    }
  }
  window.draw(groundCheckRect);
  */
}