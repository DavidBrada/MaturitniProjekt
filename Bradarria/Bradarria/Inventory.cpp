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
      container[x][y].id = x + y * xCellCount;
      container[x][y].renderBody.setSize(sf::Vector2f(renderCellSize, renderCellSize));
      container[x][y].isEmpty = true;

      container[x][y].logicBody.setSize(sf::Vector2f(logicCellSize, logicCellSize));
    }
  }
}

void Inventory::Load(sf::View view)
{
  for (int x = 0; x < xCellCount; x++)
  {
    for (int y = 0; y < yCellCount; y++)
    {
      container[x][y].renderBody.setPosition(sf::Vector2f(x * (renderCellSize + gap) + margin, y * (renderCellSize + gap) + margin));
      container[x][y].renderBody.setOutlineThickness(2);
      container[x][y].renderBody.setOutlineColor(sf::Color::Blue);

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
    }
  }
}
