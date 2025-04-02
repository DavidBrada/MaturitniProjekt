#include "Inventory.h"

void Inventory::Initialize()
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

      storedItems[index] = 0; // 0 represents no item stored

      container[x][y].logicBody.setSize(sf::Vector2f(logicCellSize, logicCellSize));
    }
  }
}

void Inventory::Load()
{
  for (int x = 0; x < xCellCount; x++)
  {
    for (int y = 0; y < yCellCount; y++)
    {
      container[x][y].renderBody.setPosition(sf::Vector2f(x * (renderCellSize + gap) + margin, y * (renderCellSize + gap) + margin));
      container[x][y].renderBody.setFillColor(sf::Color::Blue);
      container[x][y].renderBody.setOutlineThickness(2);
      container[x][y].renderBody.setOutlineColor(sf::Color::White);
    }
  }
}

void Inventory::Update(WorldGrid& worldGrid, sf::RenderWindow& window)
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
      container[x][y].itemSprite.setScale(2, 2);
      if (mousePosInventory.x == x && mousePosInventory.y == y)
      {
        container[mousePosInventory.x][mousePosInventory.y].renderBody.setOutlineThickness(4);
      }
      else
      {
        container[mousePosInventory.x][mousePosInventory.y].renderBody.setOutlineThickness(3);
      }

      if (container[x][y].quantity == 0)
      {
        storedItems[container[x][y].id] = 0;
        SetSprite(0, x, y, worldGrid.tileAtlasTexture, worldGrid.atlasTiles);
      }

      if (selectedPosition.x == x && selectedPosition.y == y)
      {
        container[selectedPosition.x][selectedPosition.y].renderBody.setOutlineColor(sf::Color::Yellow);
        container[selectedPosition.x][selectedPosition.y].renderBody.setOutlineThickness(4);
      }
      else
      {
        container[x][y].renderBody.setOutlineThickness(2);
        container[x][y].renderBody.setOutlineColor(sf::Color::White);
      }
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
  selectedPosition = sf::Vector2f(mousePosInventory.x, mousePosInventory.y);
}

void Inventory::SetSprite(int type, float xPos, float yPos, sf::Texture& tileAtlasTexture, AtlasTile*& atlasTiles)
{
  container[xPos][yPos].itemSprite.setTexture(tileAtlasTexture);
  container[xPos][yPos].itemSprite.setTextureRect(sf::IntRect(
    atlasTiles[type].position.x,
    atlasTiles[type].position.y,
    16, 16
  ));
}

void Inventory::StoreItem(int type, WorldGrid& worldGrid)
{
  for (int i = 0; i < inventorySize; i++)
  {
    if (storedItems[i] == type)
    {
      for (int x = 0; x < xCellCount; x++)
      {
        for (int y = 0; y < yCellCount; y++)
        {
          if (container[x][y].id == i)
          {
            container[x][y].quantity++;
          }
        }
      }

      break;
    }
    else if (storedItems[i] == 0)
    {
      storedItems[i] = type;

      for (int x = 0; x < xCellCount; x++)
      {
        for (int y = 0; y < yCellCount; y++)
        {
          if (container[x][y].id == i)
          {
            container[x][y].itemID = type;
            SetSprite(type, x, y, worldGrid.tileAtlasTexture, worldGrid.atlasTiles);
            container[x][y].quantity++;
          }
        }
      }
      break;
    }
  }
}

Inventory::~Inventory()
{
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
      window.draw(container[x][y].itemSprite);
    }
  }
}