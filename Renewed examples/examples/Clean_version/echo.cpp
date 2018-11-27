#include <iostream>
#include <string_view>
#include <uWS/uWS.h>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <fstream>
#include <pthread.h>
#include "MsgQueue.h"

#include "spi_communication.h"

char *s = "/dev/spi_drv0";


using namespace std;
uint16_t IDs[4];
char getData[4];
int ret;
int fd;

int stopFlag = 0;
int spiRead = 0;

MsgQueue webAppMq(10);
MsgQueue RFIDMq(10);
MsgQueue gameMq(10);


enum
{
  START_WEBAPP,
  ST_IDLE,
  ST_INIT,
  ST_READ,
  INDSAMLESPIL,
  FARVESPIL,
  FRI_STYRING,
  GAME_IDLE,
};


void wepAppHandler(Message* msg, unsigned long id)
{
  switch(id)
  {
    case START_WEBAPP:
    {
      uWS::Hub h;
      while(1)
      {
        cout << "State: START WEBAPP" << endl;
            /*Det her skal den gøre når den får en besked.*/
            h.onMessage([](uWS::WebSocket<uWS::SERVER> *ws,
                           char *message, size_t length,
                           uWS::OpCode opCode)
            {
                std::cout << "Data: " << std::string_view(message, length) << std::endl;


              switch (*message)
              {
                case 'r':
                {
                  std::cout << "Der er trykket på r" << std::endl;
                  {
                    SPIToolkit obj(s);
                    // Forhindringsbane
                    obj.writeSPI('1');
                  }
                }
                break;

                case 'l':
                {
                  std::cout << "Der er trykket på l" << std::endl;
                  {
                    SPIToolkit obj(s);
                    // Forhindringsbane
                    obj.writeSPI('2');
                  }
                }
                break;

                case 'u':
                {
                  std::cout << "Der er trykket på u" << std::endl;
                  {
                    SPIToolkit obj(s);
                    // Forhindringsbane
                    obj.writeSPI('3');
                  }
                }
                break;

                case 'd':
                {
                  std::cout << "Der er trykket på d" << std::endl;
                  {
                    SPIToolkit obj(s);
                    // Forhindringsbane
                    obj.writeSPI('4');
                  }
                }
                break;

                case 'v':
                {
                  std::cout << "Spillet stoppes" << std::endl;
                  {
                    // Sæt et globalt flag, der inde i spil handler stopper spil
                    stopFlag = 1;
                    {
                      SPIToolkit obj(s);
                      // Farvespil
                      obj.writeSPI('3');
                    }

                    //SPIToolkit obj(s);
                    // Forhindringsbane
                    //obj.writeSPI('8');
                  }
                }
                break;

                case 'z':
                {
                  fstream myFile;
                  myFile.open("farve.txt", ios::out|ios::in| ios::trunc);
                  if (myFile.is_open())
                  {
                    myFile << "gul\n";
                    myFile.close();
                  }
                }
                break;

                case 'x':
                {
                  fstream myFile;
                  myFile.open("farve.txt", ios::out|ios::in| ios::trunc);
                  if (myFile.is_open())
                  {
                    myFile << "rød\n";
                    myFile.close();
                  }
                }
                break;

                default:
                {
                  {
                    SPIToolkit obj(s);
                    // Forhindringsbane
                    obj.writeSPI('0');
                  }
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
    }
    break;

    default:
    cout << "Ukendt identifier" << endl;
  }
}

void RFIDHandler(Message* msg, unsigned long id)
{
  cout << "In RFID handler " << endl;
    switch(id)
    {
      case ST_INIT:
      {
        cout << "State: INIT" << endl;
        fd = open("/dev/wiegand", O_RDONLY, S_IRUSR);

        if(fd == -1)
        {
          printf("Error opening /dev/wiegand\n");
          exit(1);
        }
        char buf[4];
        cout << "Initialize cards. Scan now" << endl;
        for(int i = 0; i < 4; i++)
        {
          cout << "Card" << i + 1 << ":";
          ret = read(fd, buf, sizeof(buf));
          if (ret < 0)
          {
            cout << "Failed to read" << endl;
          }
          IDs[i] = atoi(buf);
          cout << IDs[i] << endl;
        }
        close(fd);
        RFIDMq.send(ST_READ);


      }
      break;

      case ST_IDLE:
      {
        // Lav ingenting, så længe der er et spil i gang. Hvis den får at
        // Vide at et spil har nået enden, eller det stoppes af bruger, skal den gå
        // I state read igen.
      }
      break;

      case ST_READ:
      {
        fd = open("/dev/wiegand", O_RDONLY, S_IRUSR);
        ret = read(fd, getData, sizeof(getData));
        if (ret < 0)
        {
          cout << "Failed to read" << endl;
        }
        close(fd);

        int result = atoi(getData);
        cout << "Recieved data: " << result << endl;

        if(result == IDs[0])
        {
          cout << "Forhindringsbane" << endl;
        }
        else if(result == IDs[1])
        {
          cout << "Fri styring" << endl;
          gameMq.send(FRI_STYRING);
        }
        else if(result == IDs[2])
        {
          cout << "Indsamlespil" << endl;
          gameMq.send(INDSAMLESPIL);
        }
        else if(result == IDs[3])
        {
          cout << "Farvespil" << endl;
          gameMq.send(FARVESPIL);
        }
        else
        {
          cout << "Invalid card" << endl;
          cout << "ID: " << result << endl;
        }

        RFIDMq.send(ST_IDLE);

        // Stå og vente på at der læses noget
      }
      break;


      default:
      cout << "Ukendt identifier" << endl;
    }
}

void gameHandler(Message* msg, unsigned long id)
{
    cout << "In GAME handler " << endl;
    switch(id)
    {
      case GAME_IDLE:
      {
        // DEtte gamemode skla den ikke gøre noget kontinuerligt
        // DEn skla køre fri styring indtil stop på webapp eller touch
      }
      break;

      case FRI_STYRING:
      {
        // DEtte gamemode skla den ikke gøre noget kontinuerligt
        // DEn skla køre fri styring indtil stop på webapp eller touch
      }
      break;

      case FARVESPIL:
      {
        // Send en gang, at spillet skal starte
        cout << "In game: Farvepil" << endl;
        {
          SPIToolkit obj(s);
          // Farvespil
          obj.writeSPI('13');
        }

        while(stopFlag != 1)
        {
          {
            SPIToolkit obj(s);
            spiRead = obj.readSPI();
          }
          // Fejl sker ved at den først skriver 3 og når den læser skriver den 0 ud.
          // Tjek som det første om der er stop
          if ((spiRead & 0b00001000) == 0)
          {
            // Spillet stoppes
            stopFlag = 1;
            cout << "Spillet stoppes " << endl;
          }
          else if ((spiRead & 0b00010000) != 0)
          {
            cout << "Grøn detekteret" << endl;
          }

          sleep(1);
        }
        // Spillet stoppes og der sendes besked til GHU
        // Denne skal gå i state ST_READ
        stopFlag = 0;
        RFIDMq.send(ST_READ);
        gameMq.send(GAME_IDLE);

        // Skal tjekke kontinuerligt om bane er nået eller om farve er detekteret
      }
      break;

      case INDSAMLESPIL:
      {
        while(stopFlag != 0)
        {
          {
            SPIToolkit obj(s);
            // Forhindringsbane
            obj.writeSPI('1');
          }
          usleep(100);
        }
        stopFlag = 0;
        // Skal tjekke kontinuerligt om bane er nået eller om farve er detekteret
      }
      break;

      default:
      cout << "Ukendt identifier" << endl;
    }
}



void* RFIDFunc(void* data)
{
  MsgQueue* mq = static_cast<MsgQueue*> (data);

  while(true)
  {
    unsigned long id;
    Message* msg = mq->recieve(id);
    RFIDHandler(msg, id);
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
    wepAppHandler(msg, id);
    delete msg;
  }
}

void* gameFunc(void* data)
{
  MsgQueue* mq = static_cast<MsgQueue*> (data);

  while(true)
  {
    unsigned long id;
    Message* msg = mq->recieve(id);
    gameHandler(msg, id);
    delete msg;
  }
}


int main()
{

    pthread_t webApp, RFID, game;

    pthread_create(&webApp, NULL, webAppFunc, &webAppMq);
    pthread_create(&RFID, NULL, RFIDFunc, &RFIDMq);
    pthread_create(&game, NULL, gameFunc, &gameMq);

    RFIDMq.send(ST_INIT);
    webAppMq.send(START_WEBAPP);

    gameMq.send(FARVESPIL);

    for(;;)
    {

    }



    return 0;
}
