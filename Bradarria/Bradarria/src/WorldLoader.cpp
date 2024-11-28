#include "WorldLoader.h"
#include <fstream>
#include <string>

void WorldLoader::Save(std::string fileName)
{
  std::ofstream file;

  file.open(fileName);
  file << "Output test." << std::endl;
  file << "0, 0, 1, 9, 7, 7" << std::endl;
  file.close();
}

void WorldLoader::Load(std::string fileName)
{
  std::string line;
  std::ifstream file(fileName);

  bool isValid = false;

  if (file.is_open())
  {
    while (std::getline(file, line))
    {
      if (!isValid)
      {
        if (line == "[World]")
        {
          isValid = true;
          continue;
        }
        else
        {
          std::cout << "Invalid world file." << std::endl;
          break;
        }
      
      }

      if(isValid)
      {
        int count = line.find('=');

        std::string variable = line.substr(0, count);
        std::string value = line.substr(count + 1, line.length() - count);

        std::cout << "Variable: " << variable << std::endl;
        std::cout << "Value: " << value << std::endl;

        break;
      }
    }

    file.close();
  }
  else
  {
    std::cout << "Unable to open file. Path: " << fileName << std::endl;
  }

}
