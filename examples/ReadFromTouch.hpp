#pragma once

#include "Globals.hpp"
//#include "Messages.hpp"
#include "TouchScreenIF.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>


class GameHandler;
class MsgQueue;

class ReadFromTouch : public Globals
{
public:
  ReadFromTouch(GameHandler* its, MsgQueue*);
  void ReadTouchHandler(Message* msg, unsigned long id);
  MsgQueue* getMsgQueue();
  void setGamePtr(GameHandler* its);
private:
  GameHandler* GamePtr;
  MsgQueue* rfidPtr;
  MsgQueue ReadTouchMq;
  TouchScreenIF TouchIF;
};
