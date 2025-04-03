#include "CraftingMenu.h"

void CraftingMenu::Load(WorldGrid*& worldGrid)
{
  for (int i = 0; i < cellCount; i++)
  {
    craftingContainer[i].id = i;
    craftingContainer[i].renderBody.setSize(sf::Vector2f(renderCellSize, renderCellSize));
    craftingContainer[i].logicBody.setSize(sf::Vector2f(logicCellSize, logicCellSize));
  }

  craftingContainer.resize(cellCount);
  for (int i = 0; i < cellCount; i++)
  {
    craftingContainer[i].renderBody.setPosition(sf::Vector2f(margin + gap, i * (renderCellSize + gap) + marginTop));
    craftingContainer[i].renderBody.setFillColor(sf::Color(70, 70, 200, 150));
    craftingContainer[i].renderBody.setOutlineThickness(2);
    craftingContainer[i].renderBody.setOutlineColor(sf::Color::White);

    craftingContainer[i].logicBody.setPosition(sf::Vector2f(margin + gap / 2, i * logicCellSize - gap / 2 + marginTop));
    craftingContainer[i].logicBody.setFillColor(sf::Color::Transparent);
    craftingContainer[i].logicBody.setOutlineThickness(2);
    craftingContainer[i].logicBody.setOutlineColor(sf::Color::Green);

    craftingContainer[i].logicRect.top = craftingContainer[i].logicBody.getPosition().y;
    craftingContainer[i].logicRect.left = craftingContainer[i].logicBody.getPosition().x;
    craftingContainer[i].logicRect.height = logicCellSize;
    craftingContainer[i].logicRect.width = logicCellSize;

    SetSprite(craftableTypes[i], i, worldGrid->tileAtlasTexture, worldGrid->atlasTiles);
  }
}

void CraftingMenu::Update(WorldGrid*& worldGrid, Inventory*& inventory, sf::RenderWindow& window)
{
  // This whole thing was put together last-minute
  workbenchCrafrable = false;
  woodenPlankCrafrable = false;

  for (int x = 0; x < inventory->xCellCount; x++)
  {
    for (int y = 0; y < inventory->yCellCount; y++)
    {
      if (inventory->container[x][y].itemID == worldGrid->treeLog && inventory->container[x][y].quantity > 0)
      {
        woodenPlankCrafrable = true;

        if (inventory->container[x][y].quantity >= 5)
        {
          workbenchCrafrable = true;
        }
        break;
      }
    }
  }

  isAccessible = inventory->open;

  if (inCrafting)
  {

    mousePosCrafting = (inventory->mousePosWindow.y + gap / 2 - marginTop) / logicCellSize;

    if (mousePosCrafting > cellCount)
    {
      mousePosCrafting = cellCount - 1;
    }

    sf::Event event;
    while (window.pollEvent(event))
    {
      switch (event.type)
      {
      case sf::Event::MouseButtonPressed:
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && isAccessible)
        {
          GetClickPos();
          for (int x = 0; x < inventory->xCellCount; x++)
          {
            for (int y = 0; y < inventory->yCellCount; y++)
            {
              if (inventory->container[x][y].itemID == 14)
              {
                if (selectedPosition == 0)
                {
                  inventory->container[x][y].quantity -= 5;
                }
                else if (selectedPosition == 1)
                {
                  inventory->container[x][y].quantity -= 1;
                }
                inventory->StoreItem(craftableTypes[selectedPosition], *worldGrid);
                break;
              }
            }
          }
        }
      }
    }

  }

  inCrafting = false;

  for (int i = 0; i < cellCount; i++)
  {
    craftingContainer[i].itemSprite.setPosition(craftingContainer[i].renderBody.getPosition());
    craftingContainer[i].itemSprite.setScale(2, 2);

    if (craftingContainer[i].logicRect.contains(sf::Vector2f(inventory->mousePosWindow)))
    {
      inCrafting = true;
      craftingContainer[i].renderBody.setOutlineThickness(4);
    }
    else
    {
      craftingContainer[i].renderBody.setOutlineThickness(2);
    }

  }
}

void CraftingMenu::GetClickPos()
{
  selectedPosition = mousePosCrafting;
}

void CraftingMenu::SetSprite(int type, int pos, sf::Texture& tileAtlasTexture, AtlasTile*& atlasTiles)
{
  craftingContainer[pos].itemSprite.setTexture(tileAtlasTexture);
  craftingContainer[pos].itemSprite.setTextureRect(sf::IntRect(
    atlasTiles[type].position.x,
    atlasTiles[type].position.y,
    16, 16
  ));
}

void CraftingMenu::Render(sf::RenderWindow& window)
{
  for (int i = 0; i < cellCount; i++)
  {
    if (isAccessible)
    {
      if (workbenchCrafrable)
      {
        window.draw(craftingContainer[i].renderBody);
        window.draw(craftingContainer[i].itemSprite);
      }
      else if (woodenPlankCrafrable)
      {
        window.draw(craftingContainer[1].renderBody);
        window.draw(craftingContainer[1].itemSprite);
      }
    }
  }
}

CraftingMenu::~CraftingMenu()
{
}
