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

#include "GameHandlerUnitIF.hpp"
#include <string_view>
#include <uWS/uWS.h>
#include <string>
#include "Messages.hpp"


class WebAppIF : public Globals
{
public:
  WebAppIF(GameHandlerUnitIF* setGamePtr);
  void wepAppHandler(Message* msg, unsigned long id);
  MsgQueue* getMsgQueue();
private:
  MsgQueue webAppMq;
  GameHandlerUnitIF* GamePtr;
  GameHandlerUnitIF GHUIF;
};
