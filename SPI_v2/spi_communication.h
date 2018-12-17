extern "C"
{
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
}

#include <iostream>
#include <string>

using namespace std;

class SPIToolkit {
  public:
    SPIToolkit (char *filename)
    {
      fd = open(filename, O_RDWR);
    }

    ~SPIToolkit()
    {
      close(fd);
    }

    int readSPI()
     {
      ret = read(fd, getData, 1);
      if (ret < 0)
      {
        printk(KERN_ALERT "Failed to read\n");
      }
      // Tjek om det er det korrekte

      printk(KERN_ALERT "Recieved data: %d\n", getData[0]);

      return getData[0];
    }

    void writeSPI(char text)
     {
      sendData[0] = text;
      ret = write(fd, &sendData[0], 2);
      if (ret < 0)
      {
        printk(KERN_ALERT "Failed to write\n");
      }
      printk(KERN_ALERT "Transmitted data: %d\n", sendData[0]);
    }

  private:
    int fd;
    int ret = 0;
    char getData[4];
    char sendData[2];
};
