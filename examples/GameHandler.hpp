#pragma once


#include "MsgQueue.h"
#include "Globals.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "SPIToolkit.hpp"
// #include "Messages.hpp"
#include "GameHandlerUnitIF.hpp"
//#include "touschScreen.hpp"


class GameHandler : public Globals
{
public:
  GameHandler();
  void setPointers(MsgQueue* = NULL, MsgQueue* = NULL, MsgQueue* = NULL);
  MsgQueue* getRFIDMq();
  MsgQueue* getMsgQueue();
  void gamePlayingHandler(Message* msg, unsigned long id);
  void stopMessage(Message* msg);
  void stateHandler(Message* msg, unsigned long id);
  void setStopFlag(int);
private:
  MsgQueue gameMq;
  MsgQueue* RFIDMqPtr;
  MsgQueue* touchStopPtr;
  MsgQueue* writeToTouchPtr;
  colorDetected* colorSend;
  sendStopToTouch* sendStopTouch;
  GameHandlerUnitIF GHUIF;
  int stopFlag;
  int spiRead = 0;
  void checkRecievedData(int);
  void stopAndInit();
  void displayColor();
  void handleRecievedData(int);
  void displayColor(char);

  //+ sendStopTouch() : void

};
