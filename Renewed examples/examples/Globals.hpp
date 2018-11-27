#pragma once

#include "MsgQueue.h"
#include <string>

class Globals
{
public:
  enum ID
  {
    START_WEBAPP,
    ST_IDLE,
    ST_READ,
    INDSAMLESPIL,
    FARVESPIL,
    FRI_STYRING,
    GAME_IDLE,
    SHOW_COLOR_TS,
    STOP_GAME,
    START_GAME,
    TOUCH_IDLE,
    SHOW_GAME,
    CHECK_START_STOP,
    CHECK_STOP,
    SEND_STOP_TO_TOUSCH,
    FORHINDRINGSBANE,
  };


  struct colorDetected : public Message
  {
    char showColor;
  };

  struct showGame : public Message
  {
    char gameName;
  };

  struct sendStopToTouch : public Message
  {
    char stopIndicator;
  };

  struct gameStartStop : public Message
  {
    unsigned long id;
  };

};
