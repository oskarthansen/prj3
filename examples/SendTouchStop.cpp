#include "SendTouchStop.hpp"


SendTouchStop::SendTouchStop() {}
void SendTouchStop::SendTouchStopHandler(Message* msg, unsigned long id)
{
  switch(id)
  {
    case SEND_STOP_TO_TOUSCH:
    {
      sendStopToTouch* sendStopTouch = static_cast<sendStopToTouch*>(msg);
      TouchIF.writeFifoStop(sendStopTouch->stopIndicator);
      std::cout << "STOP sendt til touchScreen: " << sendStopTouch->stopIndicator << std::endl;
    }
    break;
  }
}

MsgQueue* SendTouchStop::getMsgQueue()
{
  return &SendTouchStopMq;
}
