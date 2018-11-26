#pragma once

#include "MsgQueue.h"
#include "Globals.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
//#include "Messages.hpp"
#include "TouchScreenIF.hpp"


class WriteToTouch : public Globals
{
public:
  WriteToTouch(MsgQueue* gamePtr, MsgQueue* RFIDPtr);
  void writeToTouchHandler(Message* msg, unsigned long id);
  MsgQueue* getMsgQueue();
private:
  MsgQueue* RFIDMq;
  MsgQueue* GameMq;
  MsgQueue* gameMqPtr;
  MsgQueue touschScreenMq;
  TouchScreenIF TouchIF;
};
