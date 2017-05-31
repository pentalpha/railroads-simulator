#ifndef _STR_CONCURRENT_QUEUE_
#define _STR_CONCURRENT_QUEUE_

#include <queue>
#include <thread>
#include <mutex>
#include <string>

using namespace std;

class StringQueue
{
 public:
  string* pop();
  void push(string* item);

  StringQueue()=default;
  StringQueue(const StringQueue&) = delete;            // disable copying
  StringQueue& operator=(const StringQueue&) = delete; // disable assignment
 private:
  queue<string*> strings;
  mutex localMutex;
};

#endif
