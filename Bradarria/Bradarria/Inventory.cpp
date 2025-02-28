#include "Inventory.h"

void Inventory::Inicialize()
{
  xLimit = 500;
  yLimit = 50;

  logicCellSize = renderCellSize + gap;

  container.resize(xCellCount, std::vector<InventoryCell>());

  for (int x = 0; x < xCellCount; x++)
  {
    container[x].resize(xCellCount, InventoryCell());

    for (int y = 0; y < yCellCount; y++)
    {
      int index = x + y * xCellCount;
      container[x][y].id = index;
      container[x][y].renderBody.setSize(sf::Vector2f(renderCellSize, renderCellSize));
      container[x][y].isEmpty = true;
      container[x][y].quantity = 0;

      storedItems[index] = -1; // -1 represents no item stored

      container[x][y].logicBody.setSize(sf::Vector2f(logicCellSize, logicCellSize));
    }
  }
}

void Inventory::Load(sf::View& view)
{
  for (int x = 0; x < xCellCount; x++)
  {
    for (int y = 0; y < yCellCount; y++)
    {
      container[x][y].renderBody.setPosition(sf::Vector2f(x * (renderCellSize + gap) + margin, y * (renderCellSize + gap) + margin));
      container[x][y].renderBody.setFillColor(sf::Color::Blue);
      container[x][y].renderBody.setOutlineThickness(2);
      container[x][y].renderBody.setOutlineColor(sf::Color::White);

      // These won't be rednered in release DELETE LATER
      container[x][y].logicBody.setPosition(sf::Vector2f((x * logicCellSize + margin) - gap / 2, (y * logicCellSize + margin) - gap / 2));
      container[x][y].logicBody.setOutlineThickness(2);
      container[x][y].logicBody.setOutlineColor(sf::Color::Magenta);
      container[x][y].logicBody.setFillColor(sf::Color::Transparent);
    }
  }
}

void Inventory::Update(sf::View& view, WorldGrid& worldGrid, sf::RenderWindow& window)
{
  mousePosWindow = sf::Mouse::getPosition(window);

  if (inInventory)
  {
    if (mousePosWindow.x >= 0.f)
    {
      mousePosInventory.x = (mousePosWindow.x - margin + gap / 2) / logicCellSize;
    }
    if (mousePosWindow.y >= 0.f)
    {
      mousePosInventory.y = (mousePosWindow.y - margin + gap / 2) / logicCellSize;
    }

    if (mousePosInventory.x > xCellCount - 1)
    {
      mousePosInventory.x = xCellCount - 1;
    }
    if (mousePosInventory.y > yCellCount - 1)
    {
      mousePosInventory.y = yCellCount - 1;
    }
  }

  for (int x = 0; x < xCellCount; x++)
  {
    for (int y = 0; y < yCellCount; y++)
    {
      container[x][y].itemSprite.setPosition(container[x][y].renderBody.getPosition());
    }
  }

  // Change the "inInventory" range based on the actual inventory size (when it's closed, only the hotbar is visible)
  if (open)
  {
    yLimit = 250;
  }
  else
  {
    yLimit = 55;
  }

  if (mousePosWindow.x < xLimit && mousePosWindow.y < yLimit)
  {
    inInventory = true;
  }
  else
  {
    inInventory = false;
  }
}

void Inventory::GetClickPos()
{
  clickPosition = sf::Vector2f(mousePosInventory.x, mousePosInventory.y);
}

void Inventory::SetSprite(int type, float xPos, float yPos, sf::Texture& tileAtlasTexture, AtlasTile*& atlasTiles)
{
  container[xPos][yPos].itemSprite.setTexture(tileAtlasTexture);
  container[xPos][yPos].itemSprite.setTextureRect(sf::IntRect(
    atlasTiles[type].position.x,
    atlasTiles[type].position.y,
    16,
    16
  ));
}

void Inventory::Render(sf::RenderWindow& window)
{
  if (open)
  {
    for (int x = 0; x < xCellCount; x++)
    {
      for (int y = 0; y < yCellCount; y++)
      {
        window.draw(container[x][y].renderBody);
        window.draw(container[x][y].logicBody);
        window.draw(container[x][y].itemSprite);
      }
    }
  }
  else
  {
    for (int x = 0; x < xCellCount; x++)
    {
      int y = 0;
      window.draw(container[x][y].renderBody);
      window.draw(container[x][y].logicBody);
      window.draw(container[x][y].itemSprite);
    }
  }
}
