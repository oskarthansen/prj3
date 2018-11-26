
#include "Initiator.hpp"
#include "MsgQueue.h"

using namespace std;


Initiator::Initiator(MsgQueue* setPtr) : RFIDMq(10)
{
  gameMqPtr = setPtr;
}


MsgQueue* Initiator::getGameMq()
{
  return gameMqPtr;
}


MsgQueue* Initiator::getMsgQueue()
{
  return &RFIDMq;
}

void Initiator::setTouchPtr(MsgQueue* setPtr, MsgQueue* readTouch)
{
  touchPtr = setPtr;
  readTouchPtr = readTouch;
}

void Initiator::chooseGame(uint16_t IDs)
{
  showGame* gameToShow = new showGame;
  gameStartStop* gameToRun = new gameStartStop;
  switch(IDs)
  {
    case 97:
    {
      // Fri styring

      gameToShow->gameName = 'u';
      touchPtr->send(SHOW_GAME, gameToShow);

      gameToRun->id = FRI_STYRING;
      readTouchPtr->send(CHECK_START_STOP, gameToRun);
    }
    break;

    case 171:
    {
      // Forhindringsbane'
      gameToShow->gameName = 'b';
      touchPtr->send(SHOW_GAME, gameToShow);

      gameToRun->id = FORHINDRINGSBANE;
      readTouchPtr->send(CHECK_START_STOP, gameToRun);

    }
    break;

    case 195:
    {
      // Farvespil
      gameToShow->gameName = 'f';
      touchPtr->send(SHOW_GAME, gameToShow);

      gameToRun->id = FARVESPIL;
      readTouchPtr->send(CHECK_START_STOP, gameToRun);
    }
    break;

    case 201:
    {
      // Indsamlespil
      gameToShow->gameName = 'i';
      touchPtr->send(SHOW_GAME, gameToShow);

      gameToRun->id = INDSAMLESPIL;
      readTouchPtr->send(CHECK_START_STOP, gameToRun);
    }
    break;

    default:
    {
        idIF.errorInGameRead();

        gameToShow->gameName = 'e';
        touchPtr->send(SHOW_GAME, gameToShow);

        cout << "Læsefejl. Scan venligst igen" << endl;
        RFIDMq.send(ST_READ);
    }
    break;
  }
}

void Initiator::InitiatorHandler(Message* msg, unsigned long id)
{
    cout << "In RFID handler " << endl;
    switch(id)
    {
      case ID_NEW_GAME:
      {
        chooseGame(ID);
        RFIDMq.send(ST_IDLE);
      }
      break;


      case ST_IDLE:
      {
        cout << "RFID I STATE IDLE" << endl;

      }
      break;

      case ST_READ:
      {
        cout << "Read game: " << endl;
        ID = idIF.readGame();
        RFIDMq.send(ID_NEW_GAME);
      }
      break;

      default:
      cout << "Ukendt identifier" << endl;
    }
}
