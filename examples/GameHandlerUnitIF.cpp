#include "GameHandlerUnitIF.hpp"
#include "SPIToolkit.hpp"

using namespace std;

GameHandlerUnitIF::GameHandlerUnitIF()
{

}

void GameHandlerUnitIF::stopGame()
{
  cout << "Stop game" << endl;
  {
    SPIToolkit obj(s);
    // Farvespil
    obj.writeSPI(0);
  }
}

int GameHandlerUnitIF::read()
{
  {
    SPIToolkit obj(s);
    spiRead = obj.readSPI();
  }
  return spiRead;
}


void GameHandlerUnitIF::runForhindringsbane()
{
  cout << "In game: Forhindringsbane" << endl;
  {
    SPIToolkit obj(s);
    obj.writeSPI(0b00001001);
  }
}

void GameHandlerUnitIF::runIndsamlespil()
{
  cout << "In game: Indsamlespil" << endl;
  {
    SPIToolkit obj(s);
    obj.writeSPI(0b00001011);
  }
}

void GameHandlerUnitIF::runFriStyring()
{
  cout << "In game: Fri styring" << endl;
  {
    SPIToolkit obj(s);
    obj.writeSPI(0b00001101);
  }
}

void GameHandlerUnitIF::runFarvespil()
{
  cout << "In game: Farvespil" << endl;
  {
    SPIToolkit obj(s);
    obj.writeSPI(0b00001010);
  }
}

void GameHandlerUnitIF::goForwards()
{
  cout << "Go forwards" << endl;
  {
    SPIToolkit obj(s);
    obj.writeSPI(0b10001000);
  }
}

void GameHandlerUnitIF::goLeft()
{
  cout << "Go left" << endl;
  {
    SPIToolkit obj(s);
    obj.writeSPI(0b00101000);
  }
}
void GameHandlerUnitIF::goRight()
{
  cout << "Go right" << endl;
  {
    SPIToolkit obj(s);
    obj.writeSPI(0b00011000);
  }
}

void GameHandlerUnitIF::goBackwards()
{
  cout << "Go backwards" << endl;
  {
    SPIToolkit obj(s);
    obj.writeSPI(0b01001000);
  }
}

void GameHandlerUnitIF::goIdle()
{
  cout << "Stop driving" << endl;
  {
    SPIToolkit obj(s);
    obj.writeSPI(0b00001000);
  }
}
