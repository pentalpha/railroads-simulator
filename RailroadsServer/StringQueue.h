#ifndef _STR_CONCURRENT_QUEUE_
#define _STR_CONCURRENT_QUEUE_

#include <queue>
#include <thread>
#include <mutex>
#include <string>

class StringQueue
{
 public:
  std::string* pop();
  void push(std::string* item);

  StringQueue()=default;
  StringQueue(const StringQueue&) = delete;            // disable copying
  StringQueue& operator=(const StringQueue&) = delete; // disable assignment
 private:
  std::queue<std::string*> strings;
  std::mutex localMutex;
};

#endif
