/*
 * Based on code by Evgeny Zavalkovsky,
 * published under the The Code Project Open License (CPOL) 1.02
 * at https://www.codeproject.com/Articles/723656/SW-Message-Bus
 */

#pragma once

#include <map>

#include "../MessageBusDefs.h"
#include "SubscriberHandle.h"

/*
 * Non template base of MsgTypeContainer class
 * Required for omitting template parameter dependency
 * in MsgTypeContainer class
 */

class MsgTypeContainerBase {
 public:
  MsgTypeContainerBase() = default;
  virtual ~MsgTypeContainerBase() = default;
  MsgTypeContainerBase(MsgTypeContainerBase&) = delete;
  MsgTypeContainerBase(MsgTypeContainerBase&&) = delete;
  MsgTypeContainerBase& operator=(MsgTypeContainerBase&) = delete;
  MsgTypeContainerBase& operator=(MsgTypeContainerBase&&) = delete;

  virtual void remove(SubscriberHandle handle) = 0;
  virtual bool empty() = 0;
};

template <typename MSG_TYPE>
class MsgTypeContainer: public MsgTypeContainerBase {
 public:
  void add(SubscriberHandle handle, Subscriber<MSG_TYPE> subscriber) {
    containerMap.insert(MsgBusContainerMapPair(handle, subscriber));
  }

  void remove(SubscriberHandle handle) {
    containerMap.erase(handle);
  }

  bool empty() {
    return containerMap.empty();
  }

  void publish(const MSG_TYPE& msg) {
    for (auto& iter : containerMap) {
      printf("    Calling subscriber\n");
      const Subscriber<MSG_TYPE> &s = iter.second;
      s(msg);
    }
  }

  /// Default Contructor and Destructor
  //  Deleted Move and Copy Constractors and Assign Operators

  MsgTypeContainer() = default;
#ifdef __GNUC__
  virtual ~MsgTypeContainer() noexcept = default;
#else
  virtual ~MsgTypeContainer() = default;
#endif
  MsgTypeContainer(MsgTypeContainer&) = delete;
  MsgTypeContainer(MsgTypeContainer&&) = delete;
  MsgTypeContainer& operator=(MsgTypeContainer&) = delete;
  MsgTypeContainer& operator=(MsgTypeContainer&&) = delete;

 private:
  struct SubscriberHandleComparator {
    bool operator()(SubscriberHandle const& lhs, SubscriberHandle const& rhs) const {
      return lhs.getCounter() < rhs.getCounter() ? true
                                                 : (lhs.getCounter() == rhs.getCounter() ? lhs.getClock() < rhs.getClock() : false);
    }
  };

  using MsgBusContainerMap = std::map<SubscriberHandle, Subscriber<MSG_TYPE>, SubscriberHandleComparator>;

  using MsgBusContainerMapPair = std::pair<SubscriberHandle, Subscriber<MSG_TYPE>>;

  MsgBusContainerMap containerMap;
};
