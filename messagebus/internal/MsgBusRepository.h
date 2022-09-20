/*
 * Based on code by Evgeny Zavalkovsky,
 * published under the The Code Project Open License (CPOL) 1.02
 * at https://www.codeproject.com/Articles/723656/SW-Message-Bus
 */

#pragma once

#include <map>

#include "../MessageBusDefs.h"
#include "MsgTypeContainer.h"
#include "SharedMutex.h"
#include "SubscriberHandle.h"
#include "TypeId.h"

class MsgBusRepository {
public:
  MsgBusRepository() : operational(true) {}

  ~MsgBusRepository() {
    mutex.lockExclusive();

    for (auto iter : repositoryMap) {
      delete iter.second;
    }

    operational = false;
    mutex.unlockExclusive();
  }

  template <typename MSG_TYPE>
  SubscriberHandle subscribe(Subscriber<MSG_TYPE> subscriber) {
    TypeId typeId = getTypeId<MSG_TYPE>();

    mutex.lockExclusive();

    SubscriberHandleTyped<MSG_TYPE> handle;
    if (operational) {
      auto ret = repositoryMap.insert(MsgBusRepositoryMapPair(typeId, nullptr));

      /// Check if this is the first subscriber for the MSG_TYPE.
      if (ret.second) {
        ret.first->second = new MsgTypeContainer<MSG_TYPE>;
      }

      // Add subscriber to the container.
      (static_cast<MsgTypeContainer<MSG_TYPE> *>(ret.first->second))
          ->add(handle, subscriber);
    } else {
      handle.setInvalid();
    }

    mutex.unlockExclusive();

    return handle;
  }

  void unSubscribe(SubscriberHandle &handle) {
    mutex.lockExclusive();

    if (operational && handle.isValid()) {
      TypeId typeId = handle.getTypeid();

      auto iter = repositoryMap.find(typeId);

      if (iter != repositoryMap.end()) {
        MsgTypeContainerBase *container = iter->second;

        container->remove(handle);

        /// Check if this is the last subscriber in the container
        if (container->empty()) {
          repositoryMap.erase(iter);
          delete container;
        }
      }
    }

    handle.setInvalid();

    mutex.unlockExclusive();
  }

  template <typename MSG_TYPE> void publish(const MSG_TYPE &msg) {
    TypeId typeId = getTypeId<MSG_TYPE>();

    mutex.lockShared();

    if (operational) {
      auto iter = repositoryMap.find(typeId);
      if (iter != repositoryMap.end()) {
        (static_cast<MsgTypeContainer<MSG_TYPE> *>(iter->second))->publish(msg);
      }
    }

    mutex.unlockShared();
  }

  /// Disable coping and moving.

  MsgBusRepository(MsgBusRepository &) = delete;
  MsgBusRepository(MsgBusRepository &&) = delete;
  MsgBusRepository &operator=(MsgBusRepository &) = delete;
  MsgBusRepository &operator=(MsgBusRepository &&) = delete;

private:
  using MsgBusRepositoryMap = std::map<TypeId, MsgTypeContainerBase *>;
  using MsgBusRepositoryMapPair = std::pair<TypeId, MsgTypeContainerBase *>;

  bool operational;
  MsgBusRepositoryMap repositoryMap;

  /// Multiple Readers - Single Writer Lock.
  SharedMutex mutex;
};