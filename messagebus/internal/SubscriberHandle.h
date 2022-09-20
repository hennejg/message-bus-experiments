/*
 * Based on code by Evgeny Zavalkovsky,
 * published under the The Code Project Open License (CPOL) 1.02
 * at https://www.codeproject.com/Articles/723656/SW-Message-Bus
 */

#pragma once

#include <atomic>
#include <cstdint>
#include <ctime>

#include "TypeId.h"

/*
 * Represents unique handle for subscriber.
 * Generated using atomic counter and clockTime( to handle counter
 * overflow )
 */

class SubscriberHandle {
 public:
  SubscriberHandle(TypeId _typeId) : clockTime(clock()), typeId(_typeId) {
    counter = nextId();
  }

  uint32_t getCounter() const {
    return counter;
  }

  clock_t getClock() const {
    return clockTime;
  }

  TypeId getTypeid() const {
    return typeId;
  }

  void setInvalid() {
    counter = 0;
    typeId = 0;
  }

  bool isValid() {
    return ((counter != 0) && (typeId != 0));
  }

 private:
  static uint32_t nextId() {
    static std::atomic<uint32_t> commonCounter {0};
    return ++commonCounter;
  }

  uint32_t counter;
  clock_t clockTime;
  TypeId typeId;
};

template <typename MSG_TYPE>
class SubscriberHandleTyped: public SubscriberHandle {
 public:
  SubscriberHandleTyped/*<MSG_TYPE>*/() : SubscriberHandle(getTypeId<MSG_TYPE>()) {
  }
};
