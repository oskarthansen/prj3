#include "TouchScreenIF.hpp"

using namespace std;
TouchScreenIF::TouchScreenIF()
{

}

char TouchScreenIF::readFifoStopTouch()
{
  std::cout << "IN GAME STOP" << std::endl;
  fd = open("fifoStopTouch", O_RDWR);
  ret = read(fd, buf, 1);
  if (ret < 0)
  {
    std::cout << "Failed to write color in CheckTouchStop.cpp" << std::endl;
  }
  close(fd);

  return buf[0];
}


void TouchScreenIF::readFifoStart()
{
  std::cout << std::endl << "Waiting for STOP OR GO" << std::endl;
  fd = open("fifoStart", O_RDWR);
  ret = read(fd, buf, 1);
  if (ret < 0)
  {
    std::cout << "Failed to write color in touchScreen" << std::endl;
  }
  close(fd);
}


void TouchScreenIF::writeFifoGame(char sendToFifo)
{
  fd = open("fifoGame", O_WRONLY);
  ret = write(fd, &sendToFifo, sizeof(sendToFifo));
  if (ret < 0)
  {
    cout << "Failed to write color in touchScreen" << endl;
  }
  close(fd);
}


void TouchScreenIF::writeFifoColor(char sendToFifo)
{
  fd = open("fifoColor", O_WRONLY);

  ret = write(fd, &sendToFifo, sizeof(sendToFifo));
  if (ret < 0)
  {
    cout << "Failed to write color in touchScreen" << endl;
  }
  close(fd);
}


void TouchScreenIF::writeFifoStop(char sendToFifo)
{
  fd = open("fifoStop", O_WRONLY);

  ret = write(fd, &sendToFifo, sizeof(sendToFifo));
  if (ret < 0)
  {
    cout << "Failed to write STOP to touchScreen" << endl;
  }
  close(fd);
}


char TouchScreenIF::getBuf()
{
  return buf[0];
}
