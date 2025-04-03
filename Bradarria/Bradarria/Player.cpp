#include "Player.h"

void Player::Update(float& deltaTime, WorldGrid& worldGrid, sf::View& view, sf::RectangleShape& tileSelectorBody)
{
  sf::Vector2f rayOrigin = sf::Vector2f(body.getPosition().x + body.getSize().x / 2, body.getPosition().y + body.getSize().y / 2);
  sf::Vector2f rayDirection = velocity * 20.f;

  sf::Vector2f contactPoint, contactNormal;
  float time;

  std::vector<std::pair<sf::Vector2i, float>> z; // This is for sorting collision blocks 

#pragma region getInput
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
  {
    velocity.x = -moveSpeed;
    inputVelocity.x = -1;
    state = walking;

    // Rotate sprite left
    sprite.setOrigin(width, 0);
    sprite.setScale(-1.f, 1.f);
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
  {
    velocity.x = moveSpeed;
    inputVelocity.x = 1;
    state = walking;

    // Rotate sprite right
    sprite.setOrigin(0, 0);
    sprite.setScale(1.f, 1.f);
  }
  else
  {
    velocity.x = 0.f;
    inputVelocity.x = 0.f;
    state = idle;
  }
#pragma endregion

  if (jumping)
  {
    if (jumpTimer.getElapsedTime().asSeconds() < jumpTime)
    {
      jumpForce /= jumpForceFade;
      velocity.y -= jumpForce;
    }
    else
    {
      jumping = false;
      jumpForce = defaultJumpForce;
    }
  }
  if (state == idle)
  {
    if (animClock.getElapsedTime().asSeconds() >= 0.3f)
    {
      if (currentAnimFrame == animFrameCount)
      {
        currentAnimFrame = 0;
      }
      sprite.setTextureRect(sf::IntRect(currentAnimFrame * width, 0, width, height));


      currentAnimFrame++;
      animClock.restart();
    }
  }
  else if (state == walking)
  {
    if (animClock.getElapsedTime().asSeconds() >= 0.15f)
    {
      if (currentAnimFrame == animFrameCount)
      {
        currentAnimFrame = 0;
      }
      sprite.setTextureRect(sf::IntRect(currentAnimFrame * width, height, width, height));


    currentAnimFrame++;
    animClock.restart();
    }
  }
  

  canPlace = false;
  tileSelectorBody.setOutlineColor(sf::Color::Red);

  xCanPlaceFrom = worldGrid.mousePosGrid.x - 1;
  xCanPlaceTo = worldGrid.mousePosGrid.x + 2;
  yCanPlaceFrom = worldGrid.mousePosGrid.y - 1;
  yCanPlaceTo = worldGrid.mousePosGrid.y + 2;

#pragma region mapEdgeConditions
  if (xCanPlaceFrom < 0)
  {
    xCanPlaceFrom = 0;
  }
  else if (xCanPlaceFrom >= worldGrid.mapWidth)
  {
    xCanPlaceFrom = worldGrid.mapWidth - 1;
  }

  if (xCanPlaceTo < 0)
  {
    xCanPlaceTo = 0;
  }
  else if (xCanPlaceTo >= worldGrid.mapWidth)
  {
    xCanPlaceTo = worldGrid.mapWidth;
  }

  if (yCanPlaceFrom < 0)
  {
    yCanPlaceFrom = 0;
  }
  else if (yCanPlaceFrom >= worldGrid.mapHeight)
  {
    yCanPlaceFrom = worldGrid.mapHeight - 1;
  }

  if (yCanPlaceTo < 0)
  {
    yCanPlaceTo = 0;
  }
  else if (yCanPlaceTo >= worldGrid.mapHeight)
  {
    yCanPlaceTo = worldGrid.mapHeight;
  }
#pragma endregion

  for (int x = xCanPlaceFrom; x < xCanPlaceTo; x++)
  {
    for (int y = yCanPlaceFrom; y < yCanPlaceTo; y++)
    {
      if (worldGrid.tileMap[x][y].hasCollision && (worldGrid.tileMap[worldGrid.mousePosGrid.x][worldGrid.mousePosGrid.y].type == worldGrid.air ||
          worldGrid.tileMap[worldGrid.mousePosGrid.x][worldGrid.mousePosGrid.y].type == worldGrid.dirtBackground) &&
          InArea(tileSelectorBody, 10 * worldGrid.tileSize) && !InArea(tileSelectorBody, 2 * worldGrid.tileSize))
      {
        canPlace = true;
        tileSelectorBody.setOutlineColor(sf::Color::Yellow);
      }
    }
  }

  if (body.getPosition().y > worldGrid.mapHeight * worldGrid.tileSize || body.getPosition().x > worldGrid.mapWidth * worldGrid.tileSize || body.getPosition().x < 0)
  {
    body.setPosition(sf::Vector2f(worldGrid.playerSpawnPos.x, worldGrid.playerSpawnPos.y));
    view.setCenter(body.getPosition());
  }

  colliding = false;

  // Gravity
  grounded = false;

  velocity.y += gravity;

#pragma region collisionHandling
  cFromX = body.getPosition().x / worldGrid.tileSize - 3;
  cToX = body.getPosition().x / worldGrid.tileSize + 4;
  cFromY = body.getPosition().y / worldGrid.tileSize - 1;
  cToY = body.getPosition().y / worldGrid.tileSize + 5;

  if (cFromX < 0)
  {
    cFromX = 0;
  }
  if (cToX >= worldGrid.mapWidth)
  {
    cToX = worldGrid.mapWidth;
  }
  if (cFromY < 0)
  {
    cFromY = 0;
  }
  if (cToY >= worldGrid.mapHeight)
  {
    cToY = worldGrid.mapHeight;
  }

  for (int x = cFromX; x < cToX; x++)
  {
    for (int y = cFromY; y < cToY; y++)
    {
      if (worldGrid.tileMap[x][y].hasCollision)
      {
        if (IsGrounded(groundCheckRectLeft, worldGrid.tileMap[x][y].shape, contactPoint, contactNormal, time, deltaTime) ||
            IsGrounded(groundCheckRectRight, worldGrid.tileMap[x][y].shape, contactPoint, contactNormal, time, deltaTime) ||
            IsGrounded(groundCheckRectCenter, worldGrid.tileMap[x][y].shape, contactPoint, contactNormal, time, deltaTime))
        {
          if (!grounded)
          {
            grounded = true;
          }
        }

        // Put tiles that are colliding with the player and the distance from the player into a vector
        if (IsColliding(body, worldGrid.tileMap[x][y].shape, contactPoint, contactNormal, time, deltaTime))
        {
          z.push_back({ sf::Vector2i(x, y), time});
          colliding = true;
        }
      }
    }
  }

  // Sort the tiles in the vector by the calculated distance (closest tiles to the player are first)
  std::sort(z.begin(), z.end(), [](const std::pair<sf::Vector2i, float>& a, const std::pair<sf::Vector2i, float>& b)
  {
    return a.second < b.second;
  });

  // Check for collisions in sorted order
  for (auto j : z)
  {
    if (IsColliding(body, worldGrid.tileMap[j.first.x][j.first.y].shape, contactPoint, contactNormal, time, deltaTime))
    {
      velocity += sf::Vector2f(contactNormal.x * std::abs(velocity.x), contactNormal.y * std::abs(velocity.y)) * (1 - time);


    }
  }

#pragma endregion

  // Move camera with player on x
  if (velocity.x > 0.f)
  {
    view.move(moveSpeed * deltaTime, 0.f);
  }
  else if (velocity.x < 0.f)
  {
    view.move(-moveSpeed * deltaTime, 0.f);
  }

  // Move camera with player on y
  if (body.getPosition().y > view.getCenter().y + yViewMoveCenterOffset)
  {
    view.move(0.f, moveSpeed * deltaTime);
  }
  else if (body.getPosition().y < view.getCenter().y - (yViewMoveCenterOffset + 20.f))
  {
    view.move(0.f, -moveSpeed * deltaTime);
  }

  velocity.x *= deltaTime;
  velocity.y *= deltaTime;
  body.move(velocity);
  sprite.setPosition(body.getPosition());
  groundCheckRectLeft.setPosition(sf::Vector2f(body.getPosition().x, body.getPosition().y + height));
  groundCheckRectRight.setPosition(sf::Vector2f(body.getPosition().x + body.getSize().x - groundCheckRectRight.getSize().x, body.getPosition().y + height));
  groundCheckRectCenter.setPosition(sf::Vector2f(body.getPosition().x + body.getSize().x / 2 - groundCheckRectRight.getSize().x / 2, body.getPosition().y + height));
}

void Player::Jump()
{
  velocity.y = 0.f;
  jumpTimer.restart();
  jumping = true;
  velocity.y -= jumpForce;
}


bool Player::IsGrounded(const sf::RectangleShape& cBody, const sf::RectangleShape& target, sf::Vector2f& contactPoint, sf::Vector2f& contactNormal, float& contactTime, float fElapsedTime)
{
  if (RayTest(sf::Vector2f(cBody.getPosition().x + cBody.getSize().x / 2, cBody.getPosition().y + cBody.getSize().y / 2), sf::Vector2f(0.f, -1.f) * fElapsedTime, target, contactPoint, contactNormal, contactTime))
  {
    if (contactTime < 1.f) return true;
  }
  return false;
}

bool Player::InArea(sf::RectangleShape& other, int maxDistance)
{
  sf::Vector2f bodyCenterPos = sf::Vector2f(body.getPosition().x + body.getSize().x / 2, body.getPosition().y + body.getSize().y / 2);
  sf::Vector2f otherCenterPos = sf::Vector2f(other.getPosition().x + other.getSize().x / 2, other.getPosition().y + other.getSize().y / 2);
  return abs(bodyCenterPos.x - otherCenterPos.x) < maxDistance && abs(bodyCenterPos.y - otherCenterPos.y) < maxDistance;
}

// cBody is a moving rectangle, target is a static rectangle, that is being checked for collisions
// contactPoint is the nearest point where a raycast intersects with the target rectangle
// contactNormal is the normal vector of the collision (it's perpendicular to the static rectangle's surface)
// contactTime determines how far from the raycast origin did the collision occur. this value is 0 < contactTime < 1
// This function expands every tile by the player size, in order to put the player the correct distance from the tile to resolve the collision
bool Player::IsColliding(const sf::RectangleShape& cBody, const sf::RectangleShape& target, sf::Vector2f& contactPoint,
                         sf::Vector2f& contactNormal, float& contactTime, float deltaTime)
{
  if(velocity.x == 0 && velocity.y == 0) return false;

  sf::RectangleShape expandedTarget;
  expandedTarget.setPosition(target.getPosition().x - cBody.getSize().x / 2, target.getPosition().y - cBody.getSize().y / 2);
  expandedTarget.setSize(target.getSize() + cBody.getSize());

  if (RayTest(sf::Vector2f(cBody.getPosition().x + cBody.getSize().x / 2, cBody.getPosition().y + cBody.getSize().y / 2), velocity * deltaTime, expandedTarget, contactPoint, contactNormal, contactTime))
  {
    return (contactTime >= 0.0f && contactTime < 1.0f);
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

  if(std::isnan(t_far.y) || std::isnan(t_far.x)) return false;
  if(std::isnan(t_near.y) || std::isnan(t_near.x)) return false;

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
  // Uncomment commented code to see player collider and the groundCheck colliders

  //window.draw(body);
  window.draw(sprite);
  
  /*
  window.draw(groundCheckRectLeft);
  window.draw(groundCheckRectRight);
  window.draw(groundCheckRectCenter);
  //window.draw(contactNormalLine, 2, sf::Lines);
  */
}

Player::~Player()
{

}
