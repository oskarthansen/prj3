#pragma once

//#include "Message.h"
//#include "MsgQueue.h"
#include "Globals.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "GameHandler.hpp"
#include "GameHandlerUnitIF.hpp"
#include <string_view>
#include <uWS/uWS.h>
#include <string>
#include "Messages.hpp"


class WebAppIF : public Globals
{
public:
  WebAppIF(MsgQueue* setRFIDMq, MsgQueue* setTouchPtr, GameHandler*);
  void wepAppHandler(Message* msg, unsigned long id);
  MsgQueue* getMsgQueue();
private:
//  MsgQueue webAppMq;
  //MsgQueue* RFIDMq;
  GameHandler* GamePtr;
  //MsgQueue* touchStopPtr;
  GameHandlerUnitIF GHUIF;
};
