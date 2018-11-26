
#pragma once

#include <iostream>
class SPIToolkit;

class GameHandlerUnitIF
{
public:

  GameHandlerUnitIF();
  void stopGame();
  int read();
  void runForhindringsbane();
  void runIndsamlespil();
  void runFriStyring();
  void runFarvespil();

  void goForwards();
  void goLeft();
  void goRight();
  void goBackwards();
  void goIdle();

private:
  int spiRead;
  const char *s = "/dev/spi_drv0";
};
