#pragma once

#include <vector>

#include "MessageBus.h"

class Subscriptions {
  std::vector<SubscriberHandle> handles {};

 public:
  ~Subscriptions() {
    unsubscribeAll();
  }

  /**
   * @brief   Subscribe for receiving messages of the specific Message Type on the default message bus. Register the subscriber
   * handle for later unsubscription.
   * @tparam  MSG_TYPE     Type for which new subscriber will be added.
   * @param   subscriber   Callable target.
   * @return  Handle associated with a registered subscriber. Use IsValidHandle()
   *          for operation success checking.
   */
  template <typename MSG_TYPE>
  inline void subscribe(Subscriber<MSG_TYPE> subscriber) {
    registerSubscriber(MessageBus::instance().subscribe<MSG_TYPE>(subscriber));
  }

  /**
   * @brief   Subscribe for receiving messages of the specific Message Type on the default message bus. Register the subscriber
   *handle for later unsubscription.
   * @tparam  MSG_TYPE     Type for which new subscriber will be added.
   * @param   subscriber   Callable target.
   * @return  Handle associated with a registered subscriber. Use IsValidHandle()
   *          for operation success checking.
   */
  template <typename MSG_TYPE>
  inline void subscribe(Subscriber<MSG_TYPE&> subscriber) {
    registerSubscriber(MessageBus::instance().subscribe<MSG_TYPE&>(subscriber));
  }

  void registerSubscriber(SubscriberHandle subscriber) {
    handles.push_back(subscriber);
  }

  void unsubscribeAll() {
    for (auto& h : handles) {
      MessageBus::instance().unSubscribe(h);
      h.setInvalid();
    }
    handles.clear();
  }
};