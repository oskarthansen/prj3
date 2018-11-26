
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>


#include <pthread.h>
#include "ReadFromTouch.hpp"
//#include "MsgQueue.h"
#include "Globals.hpp"
#include "Initiator.hpp"
#include "GameHandler.hpp"
#include "WebAppIF.hpp"
#include "WriteToTouch.hpp"
#include "CheckTouchStop.hpp"
#include "SendTouchStop.hpp"



//Global objects

//Globals globObj;

SendTouchStop sendStopObj;
GameHandler gameObj;
Initiator rfid(gameObj.getMsgQueue());
WriteToTouch tousch(gameObj.getMsgQueue(), rfid.getMsgQueue());
WebAppIF webAppObj(rfid.getMsgQueue(), sendStopObj.getMsgQueue() , &gameObj);
ReadFromTouch readTouchObj(&gameObj, rfid.getMsgQueue());


CheckTouchStop checkTCobj(&gameObj);
//ReadTouch readTouchObj;


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
    readTouchObj.ReadTouchHandler(msg, id);
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
    rfid.InitiatorHandler(msg, id);
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
    tousch.writeToTouchHandler(msg, id);
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
    checkTCobj.CheckTouchStopHandler(msg, id);
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
    sendStopObj.SendTouchStopHandler(msg, id);
    delete msg;
  }
}


int main()
{


    pthread_t webAppThread, RFIDThread, gameThread, touschScreenTread, ReadTouchThread, CheckTouchStopThread, SendTouchStopThread;


    // MÅske laves tråd der hele tiden skal lsæe om der er trykket stop på Touch

    pthread_create(&SendTouchStopThread, NULL, SendTouchStopFunc, sendStopObj.getMsgQueue());
    pthread_create(&gameThread, NULL, gameFunc, gameObj.getMsgQueue());
    pthread_create(&touschScreenTread, NULL, WriteToTouchFunc, tousch.getMsgQueue());
    pthread_create(&webAppThread, NULL, webAppFunc, webAppObj.getMsgQueue());
    pthread_create(&RFIDThread, NULL, RFIDFunc, rfid.getMsgQueue());
    pthread_create(&CheckTouchStopThread, NULL, CheckTouchStopFunc, checkTCobj.getMsgQueue());


    webAppObj.getMsgQueue()->send(WebAppIF::START_WEBAPP);
    checkTCobj.getMsgQueue()->send(CheckTouchStop::CHECK_STOP);
    gameObj.setPointers(rfid.getMsgQueue(), sendStopObj.getMsgQueue(), tousch.getMsgQueue() );

    rfid.setTouchPtr(tousch.getMsgQueue(), readTouchObj.getMsgQueue());
    pthread_create(&ReadTouchThread, NULL, ReadTouchFunc, readTouchObj.getMsgQueue());
    rfid.getMsgQueue()->send(Initiator::ST_READ);

    for(;;)
    {

    }

    return 0;
}
