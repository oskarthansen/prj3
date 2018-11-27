#include "RFIDIF.hpp"

#define FRISTYRING_ 'u'
#define FORHINDRINGSBANE_ 'b'
#define FARVESPIL_ 'f'
#define INDSAMLESPIL_ 'i'
#define ERROR_ 'e'

using namespace std;


RFIDIF::RFIDIF(MsgQueue* setPtr) : RFIDMq(10)
{
  gameMqPtr = setPtr;
}


MsgQueue* RFIDIF::getMsgQueue()
{
  return &RFIDMq;
}

void RFIDIF::setTouchPtr(MsgQueue* setPtr, MsgQueue* readTouch)
{
  touchPtr = setPtr;
  readTouchPtr = readTouch;
}


void RFIDIF::chooseGame(uint16_t IDs)
{
  switch(IDs)
  {
    case 97:
    {
      writeGameNameTouch(FRISTYRING_);
      readStartFromTouch(FRI_STYRING);
    }
    break;

    case 171:
    {
      writeGameNameTouch(FORHINDRINGSBANE_);
      readStartFromTouch(FORHINDRINGSBANE);
    }
    break;

    case 195:
    {
      writeGameNameTouch(FARVESPIL_);
      readStartFromTouch(FARVESPIL);
    }
    break;

    case 201:
    {

      writeGameNameTouch(INDSAMLESPIL_);
      readStartFromTouch(INDSAMLESPIL);
    }
    break;

    default:
    {
        errorInGameRead();
        gameErrorMessage();

        cout << "Læsefejl. Scan venligst igen" << endl;
        RFIDMq.send(ST_READ);
    }
    break;
  }
}


void RFIDIF::RFIDHandler(Message* msg, unsigned long id)
{
    cout << "In RFID handler " << endl;
    switch(id)
    {
      case ST_IDLE:
      {
        cout << "RFID I STATE IDLE" << endl;

      }
      break;

      case ST_READ:
      {
        cout << "Read game: " << endl;
        //ID = readGame();
        ID = 97;
        chooseGame(ID);
        RFIDMq.send(ST_IDLE);
        //RFIDMq.send(ID_NEW_GAME);
      }
      break;

      default:
      cout << "Ukendt identifier" << endl;
    }
}

void RFIDIF::gameErrorMessage()
{
  showGame* gameToShow = new showGame;
  gameToShow->gameName = 'e';
  touchPtr->send(SHOW_GAME, gameToShow);
}

void RFIDIF::writeGameNameTouch(char displayGame)
{
  showGame* gameToShow = new showGame;
  gameToShow->gameName = displayGame;
  touchPtr->send(SHOW_GAME, gameToShow);
}

void RFIDIF::readStartFromTouch(unsigned long gameId)
{
  gameStartStop* gameToRun = new gameStartStop;
  gameToRun->id = gameId;
  readTouchPtr->send(CHECK_START_STOP, gameToRun);
}


int RFIDIF::readGame()
{
  fd = open("/dev/wiegand", O_RDONLY, S_IRUSR);
  if(fd == -1)
  {
    printf("Error opening /dev/wiegand\n");
    exit(1);
  }
  read(fd, buf, sizeof(buf));
  close(fd);
  ID_ = atoi(buf);
  return ID_;
}


void RFIDIF::errorInGameRead()
{
  fd = open("/dev/wiegand", O_WRONLY, S_IRUSR);
  if(fd == -1)
  {
    cout << "Error in open /dev/wiegand, write" << endl;
    exit(1);
  }
  char w[] = {'1'};
  int ret = write(fd, w, 1);
  if(ret < 0)
  {
    cout << "Error in write" << endl;
    exit(1);
  }
  close(fd);
}
