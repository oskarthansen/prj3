#pragma once

#include "Globals.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

class GameHandlerUnitIF;
class MsgQueue;

class TouchScreenIF : public Globals
{
public:

  // ReadFromTouch
  TouchScreenIF(GameHandlerUnitIF* game, MsgQueue* rfidPtr);
  void ReadTouchHandler(Message* msg, unsigned long id);
  MsgQueue* getReadTouchMsgQueue();

  //WriteToTouch
  void writeToTouchHandler(Message* msg, unsigned long id);
  MsgQueue* getWriteTouchMsgQueue();

  //SendTouchStop
  void SendTouchStopHandler(Message* msg, unsigned long id);
  MsgQueue* getTouchStopMsgQueue();

  //CheckTouchStop
  void CheckTouchStopHandler(Message* msg, unsigned long id);
  MsgQueue* getCheckTouchStopMsgQueue();

  char readFifoStopTouch();
  void readFifoStart();
  void writeFifoGame(char);
  void writeFifoColor(char);
  void writeFifoStop(char);
  char getBuf();
private:
  GameHandlerUnitIF* gamePtr_;
  MsgQueue* rfidPtr_;
  MsgQueue ReadTouchMq;
  MsgQueue touchScreenMq;
  MsgQueue SendTouchStopMq;
  MsgQueue CheckTouchStopMq;
  char buf[8];
  int fd;
  int ret;
};
