/*
 * Based on code by Evgeny Zavalkovsky,
 * published under the The Code Project Open License (CPOL) 1.02
 * at https://www.codeproject.com/Articles/723656/SW-Message-Bus
 */

#pragma once

#include <mutex>

#include "Semaphore.h"

/*
 * Mutex implementation that allows multiple Shared Locks, and single
 * Exclusive Lock. Also known as Multiple Readers -Single Writer Lock
 */
class SharedMutex {
 public:
  SharedMutex() : sharedCount(0), sharedInsideSem(1) {
  }

  ~SharedMutex() = default;

  void lockShared() {
    genEntryMutex.lock();
    genEntryMutex.unlock();
    sharedEntryMutex.lock();
    if (0 == sharedCount++) {
      sharedInsideSem.lock();
    }
    sharedEntryMutex.unlock();
  }

  void unlockShared() {
    sharedEntryMutex.lock();
    if (0 == --sharedCount) {
      sharedInsideSem.unlock();
    }
    sharedEntryMutex.unlock();
  }

  void lockExclusive() {
    genEntryMutex.lock();
    sharedInsideSem.lock();
  }

  void unlockExclusive() {
    sharedInsideSem.unlock();
    genEntryMutex.unlock();
  }

 private:
  unsigned int sharedCount;
  std::mutex genEntryMutex;
  std::mutex sharedEntryMutex;
  Semaphore sharedInsideSem;
};
