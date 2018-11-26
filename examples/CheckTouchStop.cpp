#include "CheckTouchStop.hpp"
#include "MsgQueue.h"
#include "GameHandler.hpp"


CheckTouchStop::CheckTouchStop(GameHandler* setPtr)
{
  GamePtr = setPtr;
}

void CheckTouchStop::CheckTouchStopHandler(Message* msg, unsigned long id)
{
  while(1)
  {
    switch(id)
    {
      case CHECK_STOP:
      {
        if(TouchIF.readFifoStopTouch() == 's')
        {
          GamePtr->setStopFlag(1);
        }
      }
      break;

      default:
      cout << "Ukendt identifier i Read Touch" << endl;
    }
  }
}

MsgQueue* CheckTouchStop::getMsgQueue()
{
  return &CheckTouchStopMq;
}
