#include "Queue.h"

int* Queue::pop()
{
  std::unique_lock<std::mutex> mlock(mutex_);
  if (queue_.empty())
  {
    return NULL;
  }
  int *val = queue_.front();
  queue_.pop();
  return val;
}

void Queue::push(int* item)
{
  std::unique_lock<std::mutex> mlock(mutex_);
  queue_.push(item);
  mlock.unlock();
  cond_.notify_one();
}
