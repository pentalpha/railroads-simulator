#include "StringQueue.h"

std::string* StringQueue::pop()
{
  localMutex.relock();
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
  localMutex.relock();
  strings.push(item);
  localMutex.unlock();
}
