#include "ReadFromTouch.hpp"
#include "MsgQueue.h"
#include "GameHandler.hpp"



ReadFromTouch::ReadFromTouch(GameHandler* setPtr, MsgQueue* setRFIDPtr) : ReadTouchMq(10)
{
  GamePtr = setPtr;
  rfidPtr = setRFIDPtr;
}


void ReadFromTouch::setGamePtr(GameHandler* setPtr)
{
  GamePtr = setPtr;
}

MsgQueue* ReadFromTouch::getMsgQueue()
{
  return &ReadTouchMq;
}


void ReadFromTouch::ReadTouchHandler(Message* msg, unsigned long id)
{
  std::cout << "In Read Touch handler " << std::endl;
    switch(id)
    {
      case CHECK_START_STOP:
      {
        TouchIF.readFifoStart();

        if(TouchIF.getBuf() == 'g')
        {
          // Start spil
          std::cout << std::endl << "Start GAME" << std::endl;
          gameStartStop* gameToRun = static_cast<gameStartStop*>(msg);
          GamePtr->getMsgQueue()->send(gameToRun->id);

        }
        else if(TouchIF.getBuf() == 's')
        {
          std::cout << std::endl << "Stop GAME" << std::endl;
          rfidPtr->send(ST_READ);
        }
        else
        {
          std::cout << "Ukendt char læst"  << std::endl;
        }

        // Check om der er skrevet START eller stop i et game
        // Hvis der er trykket start skal der sendes go til game om at starte
      }
      break;

      default:
      cout << "Ukendt identifier i Read Touch" << endl;
    }
}
