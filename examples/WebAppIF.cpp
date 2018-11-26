#include "WebAppIF.hpp"


WebAppIF::WebAppIF(MsgQueue* setRFIDMq, MsgQueue* setTouchPtr, GameHandler* setGamePtr) : webAppMq(10)
{
  RFIDMq = setRFIDMq;
  GamePtr = setGamePtr;
  touchStopPtr = setTouchPtr;
}

MsgQueue* WebAppIF::getMsgQueue()
{
  return &webAppMq;
}

void WebAppIF::wepAppHandler(Message* msg, unsigned long id)
{
  switch(id)
  {
    case START_WEBAPP:
    {
      uWS::Hub h;
        cout << "State: START WEBAPP" << endl;
            /*Det her skal den gøre når den får en besked.*/
            h.onMessage([this](uWS::WebSocket<uWS::SERVER> *ws,
                           char *message, size_t length,
                           uWS::OpCode opCode)
            {
                std::cout << "Data: " << std::string_view(message, length) << std::endl;


              switch (*message)
              {
                case 'r':
                {
                  GHUIF.goRight();
                }
                break;

                case 'l':
                {
                  GHUIF.goLeft();
                }
                break;

                case 'u':
                {
                  GHUIF.goForwards();
                }
                break;

                case 'd':
                {
                  GHUIF.goBackwards();
                }
                break;

                case 'v':
                {
                  std::cout << "Spillet stoppes" << std::endl;
                  {
                    GamePtr->setStopFlag(1);
                    /*
                    sendStopToTouch* sendStopTouch = new sendStopToTouch;
                    sendStopTouch->stopIndicator = 's';
                    touchStopPtr->send(SEND_STOP_TO_TOUSCH, sendStopTouch);
                    */
                  }
                }
                break;

                default:
                {
                  GHUIF.goIdle();
                }
                break;
              }
            /*Når den får en besked på tegn '2' så skal den g'te noget her*/

                /*Svarer tilbage med det samme på beskeden med echo*/
                ws->send(message, length, opCode);
            });

            if (h.listen(3000))
            {
                h.run();
            }
    }
    break;

    default:
    cout << "Ukendt identifier" << endl;
  }
}
