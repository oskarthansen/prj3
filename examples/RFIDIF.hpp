#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

#include "Globals.hpp"

class MsgQueue;

class RFIDIF: public Globals
{
public:
  RFIDIF()(MsgQueue* setPtr);
  RFIDHandler(Message* msg, unsigned long id);
  MsgQueue* getMsgQueue();
  void setTouchPtr(MsgQueue* = NULL, MsgQueue* = NULL);
  int readGame();
  void errorInGameRead();
private:
  uint16_t ID_;
  char buf[4];
  int fd;

  void chooseGame(uint16_t);
  RFIDIF idIF;
  MsgQueue RFIDMq;
  uint16_t ID = 0;
  MsgQueue* gameMqPtr;
  MsgQueue* touchPtr;
  MsgQueue* readTouchPtr;
};
