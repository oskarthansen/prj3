#include "TouchScreenIF.hpp"
#include "MsgQueue.h"
#include "GameHandlerUnitIF.hpp"

using namespace std;
TouchScreenIF::TouchScreenIF(GameHandlerUnitIF* game, MsgQueue* rfidPtr) : ReadTouchMq(10), touchScreenMq(10), SendTouchStopMq(10), CheckTouchStopMq(10)
{
  gamePtr_ = game;
  rfidPtr_ = rfidPtr;
}

void TouchScreenIF::ReadTouchHandler(Message* msg, unsigned long id)
{
  std::cout << "In Read Touch handler " << std::endl;
  switch(id)
  {
    case CHECK_START_STOP:
    {
      readFifoStart();
      if(getBuf() == 'g')
      {
        // Start spil
        std::cout << std::endl << "Start GAME" << std::endl;
        gameStartStop* gameToRun = static_cast<gameStartStop*>(msg);
        gamePtr_->getMsgQueue()->send(gameToRun->id);

      }
      else if(getBuf() == 's')
      {
        std::cout << std::endl << "Stop GAME" << std::endl;
        rfidPtr_->send(ST_READ);
      }
      else
      {
        std::cout << "Ukendt char læst"  << std::endl;
      }
    }
    break;

    default:
    cout << "Ukendt identifier i Read Touch" << endl;
  }
}

MsgQueue* TouchScreenIF::getReadTouchMsgQueue()
{
  return &ReadTouchMq;
}


// Write to Touch

void TouchScreenIF::writeToTouchHandler(Message* msg, unsigned long id)
{
  switch(id)
  {
    case TOUCH_IDLE:
    {
      std::cout << "In TOUCH_IDLE " << std::endl;
    }
    break;

    case SHOW_GAME:
    {
      // SKal vente på at der gives adgang til at starte
      std::cout << "In SHOW_GAME  " << std::endl;
      showGame* gameSend = static_cast<showGame*>(msg);
      writeFifoGame(gameSend->gameName);
      touchScreenMq.send(TOUCH_IDLE);
    }
    break;

    case SHOW_COLOR_TS:
    {

      colorDetected* colorRecieved = static_cast<colorDetected*>(msg);
      writeFifoColor(colorRecieved->showColor);
      std::cout << "Modtaget til touchScreen: " << colorRecieved->showColor << std::endl;
      touchScreenMq.send(TOUCH_IDLE);
    }
    break;

    default:
    cout << "Ukendt identifier i TOUCH handler" << endl;
  }
}


MsgQueue* TouchScreenIF::getWriteTouchMsgQueue()
{
  return &touchScreenMq;
}


// SendTouchStop

void TouchScreenIF::SendTouchStopHandler(Message* msg, unsigned long id)
{
  switch(id)
  {
    case SEND_STOP_TO_TOUSCH:
    {
      sendStopToTouch* sendStopTouch = static_cast<sendStopToTouch*>(msg);
      writeFifoStop(sendStopTouch->stopIndicator);
      std::cout << "STOP sendt til touchScreen: " << sendStopTouch->stopIndicator << std::endl;
    }
    break;
  }
}

MsgQueue* TouchScreenIF::getTouchStopMsgQueue()
{
  return &SendTouchStopMq;
}


// CheckTouchStop
void TouchScreenIF::CheckTouchStopHandler(Message* msg, unsigned long id)
{
  while(1)
  {
    switch(id)
    {
      case CHECK_STOP:
      {
        if(readFifoStopTouch() == 's')
        {
          gamePtr_->setStopFlag(1);
        }
      }
      break;

      default:
      cout << "Ukendt identifier i Read Touch" << endl;
    }
  }
}
MsgQueue* TouchScreenIF::getCheckTouchStopMsgQueue()
{
  return &CheckTouchStopMq;
}



char TouchScreenIF::readFifoStopTouch()
{
  std::cout << "IN GAME STOP" << std::endl;
  fd = open("fifoStopTouch", O_RDWR);
  ret = read(fd, buf, 1);
  if (ret < 0)
  {
    std::cout << "Failed to write color in CheckTouchStop.cpp" << std::endl;
  }
  close(fd);

  return buf[0];
}


void TouchScreenIF::readFifoStart()
{
  std::cout << std::endl << "Waiting for STOP OR GO" << std::endl;
  fd = open("fifoStart", O_RDWR);
  ret = read(fd, buf, 1);
  if (ret < 0)
  {
    std::cout << "Failed to write color in touchScreen" << std::endl;
  }
  close(fd);
}


void TouchScreenIF::writeFifoGame(char sendToFifo)
{
  fd = open("fifoGame", O_WRONLY);
  ret = write(fd, &sendToFifo, sizeof(sendToFifo));
  if (ret < 0)
  {
    cout << "Failed to write color in touchScreen" << endl;
  }
  close(fd);
}


void TouchScreenIF::writeFifoColor(char sendToFifo)
{
  fd = open("fifoColor", O_WRONLY);

  ret = write(fd, &sendToFifo, sizeof(sendToFifo));
  if (ret < 0)
  {
    cout << "Failed to write color in touchScreen" << endl;
  }
  close(fd);
}


void TouchScreenIF::writeFifoStop(char sendToFifo)
{
  fd = open("fifoStop", O_WRONLY);

  ret = write(fd, &sendToFifo, sizeof(sendToFifo));
  if (ret < 0)
  {
    cout << "Failed to write STOP to touchScreen" << endl;
  }
  close(fd);
}


char TouchScreenIF::getBuf()
{
  return buf[0];
}
