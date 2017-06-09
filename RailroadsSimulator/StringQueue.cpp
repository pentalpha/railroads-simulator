#include "StringQueue.h"

std::string StringQueue::pop()
{
  QMutexLocker locker(&localMutex);
  if (strings.empty())
  {
    return "";
  }
  std::string *val = strings.front();
  strings.pop();
  elements--;
  return std::string(val->c_str());
}

void StringQueue::push(std::string* item)
{
  QMutexLocker locker(&localMutex);
  strings.push(item);
  elements++;
}

int StringQueue::getElements(){
    QMutexLocker locker(&localMutex);
    int s = elements;
    return s;
}
