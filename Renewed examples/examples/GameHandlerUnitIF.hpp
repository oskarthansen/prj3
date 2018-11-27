#pragma once


#include "MsgQueue.h"
#include "Globals.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

// #include "Messages.hpp"
//#include "GameHandlerUnitIF.hpp"

class SPIToolkit;


class GameHandlerUnitIF : public Globals
{
public:
  GameHandlerUnitIF();
  void setPointers(MsgQueue* = NULL, MsgQueue* = NULL, MsgQueue* = NULL);
  MsgQueue* getMsgQueue();
  void gamePlayingHandler(Message* msg, unsigned long id);
  void setStopFlag(int);

  void stopGame();
  int read();
  void runForhindringsbane();
  void runIndsamlespil();
  void runFriStyring();
  void runFarvespil();

  void goForwards();
  void goLeft();
  void goRight();
  void goBackwards();
  void goIdle();

private:
  MsgQueue gameMq;
  MsgQueue* RFIDMqPtr;
  MsgQueue* touchStopPtr;
  MsgQueue* writeToTouchPtr;
  int stopFlag;
  int spiRead = 0;
  void checkRecievedData(int);
  void displayStartscreen();
  void stopAndInit();
  void displayColor(char);
  void handleRecievedData(int);

  const char *s = "/dev/spi_drv0";



  //+ sendStopTouch() : void

};
