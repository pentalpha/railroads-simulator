#include "StringQueue.h"

std::string* StringQueue::pop()
{
  std::unique_lock<std::mutex> lock(localMutex);
  if (strings.empty())
  {
    return NULL;
  }
  std::string *val = strings.front();
  strings.pop();
  lock.unlock();
  return val;
}

void StringQueue::push(std::string* item)
{
  std::unique_lock<std::mutex> lock(localMutex);
  strings.push(item);
  lock.unlock();
}
