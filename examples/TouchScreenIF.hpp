#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

class TouchScreenIF
{
public:
  TouchScreenIF();
  char readFifoStopTouch();
  void readFifoStart();
  void writeFifoGame(char);
  void writeFifoColor(char);
  void writeFifoStop(char);
  char getBuf();
private:
  char buf[8];
  int fd;
  int ret;
};
