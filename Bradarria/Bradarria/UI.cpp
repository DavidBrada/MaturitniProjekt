#include "UI.h"


void UI::Initialize()
{
  font.loadFromFile("assets/fonts/PixelifySans-VariableFont_wght.ttf");
  gridInfoText.setCharacterSize(24);
  gridInfoText.setFillColor(sf::Color(255, 10, 255));
  gridInfoText.setFont(font);
  gridInfoText.setPosition(550.f, 20.f);
  gridInfoText.setString(" NO TEXT ");

  uiText.setCharacterSize(36);
  uiText.setFillColor(sf::Color::Yellow);
  uiText.setFont(font);
  uiText.setPosition(20.f, 900.f);
  uiText.setString(" NO TEXT ");

  controlsText.setCharacterSize(36);
  controlsText.setFillColor(sf::Color::Red);
  controlsText.setFont(font);
  controlsText.setPosition(1500.f, 20.f);
  controlsText.setString(" NO TEXT ");

  playerInfoText.setCharacterSize(30);
  playerInfoText.setFillColor(sf::Color::White);
  playerInfoText.setFont(font);
  playerInfoText.setPosition(1500.f, 520.f);
  playerInfoText.setString(" NO TEXT ");

  inventoryText.setCharacterSize(16);
  inventoryText.setFillColor(sf::Color::Yellow);
  inventoryText.setFont(font);
  inventoryText.setString(" NO TEXT ");

  keyBindText.setCharacterSize(34);
  keyBindText.setFillColor(sf::Color::Red);
  keyBindText.setFont(font);
  keyBindText.setPosition(1450.f, 20.f);
  keyBindText.setString(" NO TEXT ");
}

void UI::Update(WorldGrid& worldGrid, TileSelector& tileSelector, Player& player, Inventory& inventory, CraftingMenu& craftingMenu)
{
  std::stringstream ss;
  Tile& hoveredTile = worldGrid.tileMap[worldGrid.mousePosGrid.x][worldGrid.mousePosGrid.y];
  std::string collisionStatus = hoveredTile.hasCollision ? "Yes" : "No";

  // Mouse positions and world info
  ss << "Window: " << worldGrid.mousePosWindow.x << ", " << worldGrid.mousePosWindow.y << std::endl
    << "View: " << worldGrid.mousePosView.x << ", " << worldGrid.mousePosView.y << std::endl
    << "Grid: " << worldGrid.mousePosGrid.x << ", " << worldGrid.mousePosGrid.y << std::endl
    << "Block: " << worldGrid.blocks[hoveredTile.type] << std::endl
    << "Collision: " << collisionStatus << std::endl
    << "World seed: " << worldGrid.worldSeed;
  gridInfoText.setString(ss.str());
  ss.str(std::string());

  // GUI placeholder
  ss << "Selected block: " << worldGrid.blocks[tileSelector.selectedType] << std::endl;
  uiText.setString(ss.str());
  ss.str(std::string());

  // Controls
  ss << "Show keybinds: K";
  controlsText.setString(ss.str());
  ss.str(std::string());

  //Player info
  ss << "Grounded: " << player.grounded << std::endl
    << "Colliding: " << player.colliding << std::endl
    << "X: " << std::floor((player.body.getPosition().x + player.width) / worldGrid.tileSize) << std::endl
    << "Y " << std::floor((player.body.getPosition().y + player.height) / worldGrid.tileSize) << std::endl
    << "Velocity X: " << player.velocity.x << std::endl
    << "Velocity Y: " << player.velocity.y << std::endl
    << "Jumping: " << player.jumping << std::endl
    << "Input vel X: " << player.inputVelocity.x << std::endl
    << "Click Pos: " << tileSelector.clickPosition.x << ", " << tileSelector.clickPosition.y << std::endl
    << "Crafting selected pos: " << craftingMenu.selectedPosition << std::endl
    << "Inventory pos: " << inventory.mousePosInventory.x << ", " << inventory.mousePosInventory.y << std::endl
    << "Selected item ID: " << inventory.container[inventory.selectedPosition.x][inventory.selectedPosition.y].itemID << std::endl
    << "Inv. click pos: " << inventory.selectedPosition.x << ", " << inventory.selectedPosition.y << "; ID: " << inventory.container[inventory.selectedPosition.x][inventory.selectedPosition.y].id << std::endl
    << "Mining: " << player.mining << std::endl;

  playerInfoText.setString(ss.str());
  ss.str(std::string());

  ss << "Move: A, D" << std::endl
    << "Jump: SPACE" << std::endl
    << "Build / Destroy: LMB" << std::endl
    << "Inventory: I" << std::endl
    << "Navigate inventory: 1, 2, 3..." << std::endl
    << "Debug info: TAB" << std::endl
    << "Exit game: ESC" << std::endl;
  keyBindText.setString(ss.str());
  ss.str(std::string());
}

void UI::UpdateInventory(Inventory& inventory, sf::RenderWindow& window)
{
  std::stringstream ss;

  if (inventory.open)
  {
    for (int y = 0; y < inventory.yCellCount; y++)
    {
      for (int x = 0; x < inventory.xCellCount; x++)
      {
        if (inventory.container[x][y].quantity > 0)
        {
          ss << inventory.container[x][y].quantity;
          inventoryText.setString(ss.str());
          inventoryText.setPosition(sf::Vector2f((x * (inventory.renderCellSize + inventory.gap) + inventory.margin) + inventory.renderCellSize / 3, (y * (inventory.renderCellSize + inventory.gap) + inventory.margin) + inventory.renderCellSize / 3));
          window.draw(inventoryText);
          ss.str(std::string());
        }
      }
    }
  }
  else
  {
    int y = 0;
    for (int x = 0; x < inventory.xCellCount; x++)
    {
      if (inventory.container[x][y].quantity > 0)
      {
        ss << inventory.container[x][y].quantity;
        inventoryText.setString(ss.str());
        inventoryText.setPosition(sf::Vector2f((x * (inventory.renderCellSize + inventory.gap) + inventory.margin) + inventory.renderCellSize / 3, (y * (inventory.renderCellSize + inventory.gap) + inventory.margin) + inventory.renderCellSize / 3));
        window.draw(inventoryText);
        ss.str(std::string());
      }
    }
  }

}

void UI::Render(sf::RenderWindow& window, Inventory& inventory)
{
  if (visible)
  {
    window.draw(gridInfoText);
    window.draw(playerInfoText);
    window.draw(uiText);
  }
  if (showKeyBinds)
  {
    window.draw(keyBindText);
  }
  else
  {
    window.draw(controlsText);
  }
}

UI::~UI()
{
}
