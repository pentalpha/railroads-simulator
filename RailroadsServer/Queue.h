//
// Copyright (c) 2013 Juan Palacios juan.palacios.puyana@gmail.com
// Subject to the BSD 2-Clause License
// - see < http://opensource.org/licenses/BSD-2-Clause>
//https://github.com/juanchopanza/cppblog/blob/master/Concurrency/Queue/Queue.h

#ifndef CONCURRENT_QUEUE_
#define CONCURRENT_QUEUE_

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

class Queue
{
 public:
  int* pop();

  void push(int* item);

  Queue()=default;
  Queue(const Queue&) = delete;            // disable copying
  Queue& operator=(const Queue&) = delete; // disable assignment

 private:
  std::queue<int*> queue_;
  std::mutex mutex_;
  std::condition_variable cond_;
};

#endif
