#include "WriteToTouch.hpp"

WriteToTouch::WriteToTouch(MsgQueue* gamePtr, MsgQueue* RFIDPtr) : touschScreenMq(10)
{
  RFIDMq = RFIDPtr;
  GameMq = gamePtr;
}

void WriteToTouch::writeToTouchHandler(Message* msg, unsigned long id)
{
    switch(id)
    {
      case TOUCH_IDLE:
      {
        std::cout << "In TOUCH_IDLE " << std::endl;
      }
      break;

      case SHOW_GAME:
      {
        // SKal vente på at der gives adgang til at starte
        std::cout << "In SHOW_GAME  " << std::endl;
        showGame* gameSend = static_cast<showGame*>(msg);
        TouchIF.writeFifoGame(gameSend->gameName);
        touschScreenMq.send(TOUCH_IDLE);
      }
      break;

      case SHOW_COLOR_TS:
      {

        colorDetected* colorRecieved = static_cast<colorDetected*>(msg);
        TouchIF.writeFifoColor(colorRecieved->showColor);
        std::cout << "Modtaget til touchScreen: " << colorRecieved->showColor << std::endl;

        touschScreenMq.send(TOUCH_IDLE);
      }
      break;

      default:
      cout << "Ukendt identifier i TOUCH handler" << endl;
    }

}



MsgQueue* WriteToTouch::getMsgQueue()
{
  return &touschScreenMq;
}
