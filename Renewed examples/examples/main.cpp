
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>


#include <pthread.h>
#include "Globals.hpp"

#include "RFIDIF.hpp"
#include "GameHandlerUnitIF.hpp"
#include "WebAppIF.hpp"
#include "TouchScreenIF.hpp"



GameHandlerUnitIF gameObj;
RFIDIF rfid(gameObj.getMsgQueue());
TouchScreenIF readFromTouch(&gameObj, rfid.getMsgQueue());
TouchScreenIF writeToTouch(&gameObj, rfid.getMsgQueue());
TouchScreenIF checkTouchStop(&gameObj, rfid.getMsgQueue());
TouchScreenIF sendTouchStop(&gameObj, rfid.getMsgQueue());
WebAppIF webAppObj(&gameObj);


void* gameFunc(void* data)
{
  MsgQueue* mq = static_cast<MsgQueue*> (data);

  while(true)
  {
    unsigned long id;
    Message* msg = mq->recieve(id);
    gameObj.gamePlayingHandler(msg, id);
    delete msg;
  }
}

void* ReadTouchFunc(void* data)
{
  MsgQueue* mq = static_cast<MsgQueue*> (data);

  while(true)
  {
    unsigned long id;
    Message* msg = mq->recieve(id);
    readFromTouch.ReadTouchHandler(msg, id);
    delete msg;
  }
}


void* RFIDFunc(void* data)
{
  MsgQueue* mq = static_cast<MsgQueue*> (data);

  while(true)
  {
    unsigned long id;
    Message* msg = mq->recieve(id);
    rfid.RFIDHandler(msg, id);
    delete msg;
  }
}

void* webAppFunc(void* data)
{
  MsgQueue* mq = static_cast<MsgQueue*> (data);

  while(true)
  {
    unsigned long id;
    Message* msg = mq->recieve(id);
    webAppObj.wepAppHandler(msg, id);
    delete msg;
  }
}

void* WriteToTouchFunc(void* data)
{
  MsgQueue* mq = static_cast<MsgQueue*> (data);

  while(true)
  {
    unsigned long id;
    Message* msg = mq->recieve(id);
    writeToTouch.writeToTouchHandler(msg, id);
    delete msg;
  }
}

void* CheckTouchStopFunc(void* data)
{
  MsgQueue* mq = static_cast<MsgQueue*> (data);

  while(true)
  {
    unsigned long id;
    Message* msg = mq->recieve(id);
    checkTouchStop.CheckTouchStopHandler(msg, id);
    delete msg;
  }
}

void* SendTouchStopFunc(void* data)
{
  MsgQueue* mq = static_cast<MsgQueue*> (data);

  while(true)
  {
    unsigned long id;
    Message* msg = mq->recieve(id);
    sendTouchStop.SendTouchStopHandler(msg, id);
    delete msg;
  }
}


int main()
{


    pthread_t webAppThread, RFIDThread, gameThread, touschScreenTread, ReadTouchThread, CheckTouchStopThread, SendTouchStopThread;

    pthread_create(&SendTouchStopThread, NULL, SendTouchStopFunc, sendTouchStop.getTouchStopMsgQueue());
    pthread_create(&gameThread, NULL, gameFunc, gameObj.getMsgQueue());
    pthread_create(&touschScreenTread, NULL, WriteToTouchFunc, writeToTouch.getWriteTouchMsgQueue());
    pthread_create(&webAppThread, NULL, webAppFunc, webAppObj.getMsgQueue());
    pthread_create(&RFIDThread, NULL, RFIDFunc, rfid.getMsgQueue());
    pthread_create(&CheckTouchStopThread, NULL, CheckTouchStopFunc, checkTouchStop.getCheckTouchStopMsgQueue());


    webAppObj.getMsgQueue()->send(WebAppIF::START_WEBAPP);
    checkTouchStop.getCheckTouchStopMsgQueue()->send(TouchScreenIF::CHECK_STOP);
    gameObj.setPointers(rfid.getMsgQueue(), sendTouchStop.getTouchStopMsgQueue(), writeToTouch.getWriteTouchMsgQueue());

    rfid.setTouchPtr(writeToTouch.getWriteTouchMsgQueue(), readFromTouch.getReadTouchMsgQueue());
    pthread_create(&ReadTouchThread, NULL, ReadTouchFunc, readFromTouch.getReadTouchMsgQueue());
    rfid.getMsgQueue()->send(RFIDIF::ST_READ);

    for(;;)
    {

    }

    return 0;
}
