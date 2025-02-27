#include "LoadingScreen.h"

void LoadingScreen::Initialize()
{
  if (loadingScreenTexture.loadFromFile("assets/textures/loading_screen_placeholder.png"))
  {
    sprite.setTexture(loadingScreenTexture);

    std::cout << "Successfully loaded loading screen" << std::endl;
  }
  else
  {
    std::cout << "Failed to load loading screen" << std::endl;
  }
}
