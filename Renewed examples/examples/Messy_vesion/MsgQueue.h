#include <queue>
#include "Message.h"


#include <pthread.h>



using namespace std;

class MsgQueue
{
public:

MsgQueue(unsigned long maxSize = 10)
{

  maxSize_ = maxSize;
}

void send(unsigned long id, Message* msg = NULL)
{
  pthread_mutex_lock(&locking);

  while (data.size() == maxSize_)
  {
    pthread_cond_wait(&signaling,&locking);
  }

  Item temp;
  temp.id_ = id;
  temp.msg_ = msg;

  data.push(temp);

  // Vække signaler igen

  pthread_cond_signal(&signaling);
  pthread_mutex_unlock(&locking);
}

Message* recieve(unsigned long &id)
{
  pthread_mutex_lock(&locking);

  while (data.size() == 0)
  {
    pthread_cond_wait(&signaling,&locking);
  }

  Item temp = data.front();
  Message* ptr = temp.msg_;
  id = temp.id_;

  data.pop();

  pthread_cond_signal(&signaling);
  pthread_mutex_unlock(&locking);


  return ptr;

}


~MsgQueue ()
{

}


private:
  struct Item
  {
    unsigned long id_;
    Message* msg_;
  };

  pthread_mutex_t locking = PTHREAD_MUTEX_INITIALIZER;

  pthread_cond_t signaling = PTHREAD_COND_INITIALIZER;

  queue<Item> data;
// Plus other relevant variables
  unsigned long maxSize_;


};
