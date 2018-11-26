#pragma once


#include "Globals.hpp"
//#include "Messages.hpp"
#include "RFIDIF.hpp"


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>


class MsgQueue;

class Initiator : public Globals
{
public:
  Initiator(MsgQueue* setPtr);
  MsgQueue* getGameMq();
  void InitiatorHandler(Message* msg, unsigned long id);
  MsgQueue* getMsgQueue();
  void setTouchPtr(MsgQueue* = NULL, MsgQueue* = NULL);

private:
  void chooseGame(uint16_t);
  RFIDIF idIF;
  MsgQueue RFIDMq;
  uint16_t ID = 0;
  MsgQueue* gameMqPtr;
  MsgQueue* touchPtr;
  MsgQueue* readTouchPtr;
};
