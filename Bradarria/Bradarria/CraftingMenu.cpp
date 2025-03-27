#include "CraftingMenu.h"

void CraftingMenu::AddItem(int itemID, WorldGrid*& worldGrid)
{
  for (int i = 0; i < cellCount; i++)
  {
    if (craftingContainer[i].craftItemID == 0)
    {
      SetSprite(itemID, i, worldGrid->tileAtlasTexture, worldGrid->atlasTiles);
    }
  }
}

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
    craftingContainer[i].renderBody.setFillColor(sf::Color::Yellow);
    craftingContainer[i].renderBody.setOutlineThickness(2);
    craftingContainer[i].renderBody.setOutlineColor(sf::Color::Black);

    craftingContainer[i].logicBody.setPosition(sf::Vector2f(margin + gap / 2, i * renderCellSize - gap / 2 + marginTop));
    craftingContainer[i].logicBody.setFillColor(sf::Color::Transparent);
    craftingContainer[i].logicBody.setOutlineThickness(2);
    craftingContainer[i].logicBody.setOutlineColor(sf::Color::Green);
    
    craftingContainer[i].logicRect.top = craftingContainer[i].logicBody.getPosition().y;
    craftingContainer[i].logicRect.left = craftingContainer[i].logicBody.getPosition().x;
    craftingContainer[i].logicRect.height = logicCellSize;
    craftingContainer[i].logicRect.width = logicCellSize;


    AddItem(craftableTypes[i], worldGrid);
  }
}

void CraftingMenu::Update(WorldGrid*& worldGrid, Inventory*& inventory)
{
  if (inCrafting)
  {

    if (inventory->mousePosWindow.y >= 0.f)
    {
      mousePosCrafting = (inventory->mousePosWindow.y + gap / 2 - marginTop) / logicCellSize;
    }

    if (mousePosCrafting > cellCount - 1)
    {
      mousePosCrafting = cellCount - 1;
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
      GetClickPos();
      inventory->StoreItem(craftableTypes[selectedPosition], *worldGrid);
    }
  }

  for (int i = 0; i < cellCount; i++)
  {
    craftingContainer[i].itemSprite.setPosition(craftingContainer[i].renderBody.getPosition());
    craftingContainer[i].itemSprite.setScale(2, 2);

    if (selectedPosition == i)
    {
      craftingContainer[i].renderBody.setOutlineColor(sf::Color::Red);
      craftingContainer[i].renderBody.setOutlineThickness(4);
    }
    else
    {
      craftingContainer[i].renderBody.setOutlineColor(sf::Color::Black);
      craftingContainer[i].renderBody.setOutlineThickness(2);
    }

    if (craftingContainer[i].logicRect.contains(sf::Vector2f(inventory->mousePosWindow)))
    {
      inCrafting = true;
    }
    else
    {
      inCrafting = false;
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
    window.draw(craftingContainer[i].renderBody);
    window.draw(craftingContainer[i].logicBody);
    window.draw(craftingContainer[i].itemSprite);
  }
}

CraftingMenu::~CraftingMenu()
{
}
