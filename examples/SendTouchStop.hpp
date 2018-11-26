#pragma once

#include "Globals.hpp"
//#include "Messages.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "TouchScreenIF.hpp"

class SendTouchStop : public Globals
{
public:
  SendTouchStop();
  void SendTouchStopHandler(Message* msg, unsigned long id);
  MsgQueue* getMsgQueue();
private:
  MsgQueue SendTouchStopMq;
  TouchScreenIF TouchIF;
};
