#pragma once

#include "Globals.hpp"
//  #include "Messages.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "TouchScreenIF.hpp"


class GameHandler;
class MsgQueue;

class CheckTouchStop : public Globals
{
public:
  CheckTouchStop(GameHandler* its);
  void CheckTouchStopHandler(Message* msg, unsigned long id);
  MsgQueue* getMsgQueue();
private:
  GameHandler* GamePtr;
  MsgQueue CheckTouchStopMq;
  TouchScreenIF TouchIF;
};
