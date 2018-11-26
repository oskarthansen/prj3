#include "GameHandler.hpp"


using namespace std;


GameHandler::GameHandler() : gameMq(10)
{

}

MsgQueue* GameHandler::getRFIDMq()
{
  return RFIDMqPtr;
}

MsgQueue* GameHandler::getMsgQueue()
{
  return &gameMq;
}

void GameHandler::setStopFlag(int setFlag)
{
  stopFlag = setFlag;
}


void GameHandler::setPointers(MsgQueue* RFIDPtr, MsgQueue* setTouschPtr, MsgQueue* setWritePtr)
{
  RFIDMqPtr = RFIDPtr;
  touchStopPtr = setTouschPtr;
  writeToTouchPtr = setWritePtr;
}

void GameHandler::stateHandler(Message* msg, unsigned long id)
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
          GHUIF.runFriStyring();
          while(stopFlag != 1) {}

          GHUIF.stopGame();
          RFIDMqPtr->send(ST_READ);
          gameMq.send(GAME_IDLE);

          sendStopTouch = new sendStopToTouch;
          sendStopTouch->stopIndicator = 's';
          touchStopPtr->send(SEND_STOP_TO_TOUSCH, sendStopTouch);
        }
        break;

        case FARVESPIL:
        {
          GHUIF.runFarvespil();

          while(stopFlag != 1)
          {
            spiRead = GHUIF.read();

            // Fejl sker ved at den først skriver 3 og når den læser skriver den 0 ud.
            // Tjek som det første om der er stop
            handleRecievedData(spiRead);

            sleep(1);

          }

          // Spillet stoppes og der sendes besked til GHU
          // Denne skal gå i state ST_READ

          // Send besked til TOUCH om at vise stop

          /*
          sendStopTouch->stopIndicator = 's';
          touchPtr->send(SEND_STOP_TO_TOUSCH, sendStopTouch);
*/
          sendStopTouch = new sendStopToTouch;
          sendStopTouch->stopIndicator = 's';
          touchStopPtr->send(SEND_STOP_TO_TOUSCH, sendStopTouch);

          //DE SKAL SAMLES TIL stopAndInit(; metode TIL STOP GAME
          GHUIF.stopGame();
          RFIDMqPtr->send(ST_READ);
          gameMq.send(GAME_IDLE);

          // Skal tjekke kontinuerligt om bane er nået eller om farve er detekteret
        }
        break;

        case INDSAMLESPIL:
        {
          GHUIF.runIndsamlespil();

          while(stopFlag != 1)
          {
            spiRead = GHUIF.read();

            if ((spiRead & 0b00001000) == 0)
            {
              // Spillet stoppes
              stopFlag = 1;
              cout << "Spillet stoppes " << endl;
            }


            // Fejl sker ved at den først skriver 3 og når den læser skriver den 0 ud.
            // Tjek som det første om der er stop

            sleep(1);
          }

          GHUIF.stopGame();
          RFIDMqPtr->send(ST_READ);
          gameMq.send(GAME_IDLE);

          sendStopTouch = new sendStopToTouch;
          sendStopTouch->stopIndicator = 's';
          touchStopPtr->send(SEND_STOP_TO_TOUSCH, sendStopTouch);


        }
        break;

        case FORHINDRINGSBANE:
        {
          GHUIF.runForhindringsbane();

          while(stopFlag != 1)
          {
            spiRead = GHUIF.read();

            // Tjek om der er kommet et stop

            sleep(1);


          }

          GHUIF.stopGame();
          RFIDMqPtr->send(ST_READ);
          gameMq.send(GAME_IDLE);


          // LAV DETTE OM TIL EN METODE
          sendStopTouch = new sendStopToTouch;
          sendStopTouch->stopIndicator = 's';
          touchStopPtr->send(SEND_STOP_TO_TOUSCH, sendStopTouch);

        }
        break;

        default:
        cout << "Ukendt identifier" << endl;
      }
}

void GameHandler::gamePlayingHandler(Message* msg, unsigned long id)
{
  //stopMessage(msg);
  stateHandler(msg, id);
}

void GameHandler::handleRecievedData(int data)
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
    colorSend = new colorDetected;
    colorSend->showColor = 'b';
    writeToTouchPtr->send(SHOW_COLOR_TS, colorSend);
  }
  else if ((spiRead & 0b00100000) != 0)
  {

  //  displayColor();

    cout << "Rød detekteret" << endl;
    colorSend = new colorDetected;
    colorSend->showColor = 'r';
    writeToTouchPtr->send(SHOW_COLOR_TS, colorSend);
  }
  else if ((spiRead & 0b01000000) != 0)
  {
    cout << "Gul detekteret" << endl;
    colorSend = new colorDetected;
    colorSend->showColor = 'y';
    writeToTouchPtr->send(SHOW_COLOR_TS, colorSend);
  }
  else
  {
    cout << "Ingen valid data modtaget " << endl;
  }
}


//checkRecievedData(int);
