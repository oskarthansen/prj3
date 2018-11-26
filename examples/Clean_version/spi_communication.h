extern "C"
{
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
}

// Libraries her skal mindskes


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
      ret = read(fd, getData, sizeof(getData));
      if (ret < 0)
      {
        printf("Failed to read\n");
      }
      // Tjek om det er det korrekte
      int result = atoi(getData);
      printf("Recieved data: %d\n", result);

      return result;
    }

    void writeSPI(char text)
     {
      sendData[0] = text;
      ret = write(fd, &sendData[0], 1);
      if (ret < 0)
      {
        printf("Failed to write\n");
      }
      printf("Transmitted data: %c\n", sendData[0]);
    }

  private:
    int fd;
    int ret = 0;
    char getData[4];
    char sendData[2];
};
