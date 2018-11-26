#include "RFIDIF.hpp"

using namespace std;


RFIDIF::RFIDIF(MsgQueue* setPtr) : RFIDMq(10)
{
  gameMqPtr = setPtr;
}


MsgQueue* Initiator::getMsgQueue()
{
  return &RFIDMq;
}

void Initiator::setTouchPtr(MsgQueue* setPtr, MsgQueue* readTouch)
{
  touchPtr = setPtr;
  readTouchPtr = readTouch;
}

int RFIDIF::readGame()
{
  fd = open("/dev/wiegand", O_RDONLY, S_IRUSR);
  if(fd == -1)
  {
    printf("Error opening /dev/wiegand\n");
    exit(1);
  }
  read(fd, buf, sizeof(buf));
  close(fd);
  ID_ = atoi(buf);
  return ID_;
}


void RFIDIF::errorInGameRead()
{
  fd = open("/dev/wiegand", O_WRONLY, S_IRUSR);
  if(fd == -1)
  {
    cout << "Error in open /dev/wiegand, write" << endl;
    exit(1);
  }
  char w[] = {'1'};
  int ret = write(fd, w, 1);
  if(ret < 0)
  {
    cout << "Error in write" << endl;
    exit(1);
  }
  close(fd);
}
