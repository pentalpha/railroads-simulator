#include "StringQueue.h"

std::string* StringQueue::pop()
{
  localMutex.lock();
  if (strings.empty())
  {
    return NULL;
  }
  std::string *val = strings.front();
  strings.pop();
  localMutex.unlock();
  return val;
}

void StringQueue::push(std::string* item)
{
  localMutex.lock();
  strings.push(item);
  localMutex.unlock();
}
