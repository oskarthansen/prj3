#include "GameHandlerUnitIF.hpp"
#include "SPIToolkit.hpp"

#define YELLOW 'y'
#define RED 'r'
#define BLUE 'b'


using namespace std;


GameHandlerUnitIF::GameHandlerUnitIF() : gameMq(10)
{

}

MsgQueue* GameHandlerUnitIF::getMsgQueue()
{
  return &gameMq;
}

void GameHandlerUnitIF::setStopFlag(int setFlag)
{
  stopFlag = setFlag;
}


void GameHandlerUnitIF::setPointers(MsgQueue* RFIDPtr, MsgQueue* setTouschPtr, MsgQueue* setWritePtr)
{
  RFIDMqPtr = RFIDPtr;
  touchStopPtr = setTouschPtr;
  writeToTouchPtr = setWritePtr;
}

void GameHandlerUnitIF::gamePlayingHandler(Message* msg, unsigned long id)
{
  cout << "In GAME handler " << endl;

  switch(id)
  {
    case GAME_IDLE:
    {
      std::cout << "In game idle" << std::endl;
      stopFlag = 0;
    }
    break;

    case FRI_STYRING:
    {

      cout << "In game: Fri styring" << endl;
      runFriStyring();
      while(stopFlag != 1) {}

      stopAndInit();
      displayStartscreen();
    }
    break;

    case FARVESPIL:
    {
      runFarvespil();

      while(stopFlag != 1)
      {
        spiRead = read();
        handleRecievedData(spiRead);
        sleep(1);
      }
      stopAndInit();
      displayStartscreen();

    }
    break;

    case INDSAMLESPIL:
    {
      runIndsamlespil();

      while(stopFlag != 1)
      {
        spiRead = read();
        checkRecievedData(spiRead);
        sleep(1);
      }
      stopAndInit();
      displayStartscreen();
    }
    break;

    case FORHINDRINGSBANE:
    {
      runForhindringsbane();

      while(stopFlag != 1)
      {
        spiRead = read();
        checkRecievedData(spiRead);
        sleep(1);
      }
      stopAndInit();
      displayStartscreen();
    }
    break;

    default:
    cout << "Ukendt identifier" << endl;
  }
}

void GameHandlerUnitIF::handleRecievedData(int data)
{
  if ((data & 0b00001000) == 0)
  {
    // Spillet stoppes
    stopFlag = 1;
    cout << "Spillet stoppes " << endl;
  }
  else if ((data & 0b00010000) != 0)
  {
    cout << "Blå detekteret" << endl;
    displayColor(BLUE);
  }
  else if ((data & 0b00100000) != 0)
  {
    cout << "Rød detekteret" << endl;
    displayColor(RED);
  }
  else if ((data & 0b01000000) != 0)
  {
    cout << "Gul detekteret" << endl;
    displayColor(YELLOW);
  }
  else
  {
    cout << "Ingen valid data modtaget " << endl;
  }
}

void GameHandlerUnitIF::displayColor(char colorDisplay)
{
  colorDetected* colorSend = new colorDetected;
  colorSend->showColor = colorDisplay;
  writeToTouchPtr->send(SHOW_COLOR_TS, colorSend);
}

void GameHandlerUnitIF::checkRecievedData(int checkData)
{
  if ((checkData & 0b00001000) == 0)
  {
    // Spillet stoppes
    stopFlag = 1;
    cout << "Spillet stoppes " << endl;
  }
}

void GameHandlerUnitIF::stopAndInit()
{
  stopGame();
  RFIDMqPtr->send(ST_READ);
  gameMq.send(GAME_IDLE);
}

void GameHandlerUnitIF::displayStartscreen()
{
  sendStopToTouch* sendStopTouch = new sendStopToTouch;
  sendStopTouch->stopIndicator = 's';
  touchStopPtr->send(SEND_STOP_TO_TOUSCH, sendStopTouch);
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
