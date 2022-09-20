/*
 * Based on code by Evgeny Zavalkovsky,
 * published under the The Code Project Open License (CPOL) 1.02
 * at https://www.codeproject.com/Articles/723656/SW-Message-Bus
 */

#pragma once

#include <future>

#include "./internal/MsgBusRepository.h"
#include "MessageBusDefs.h"

class MessageBus {
 public:

   /**
   * @brief   Subscribe for receiving messages of the specific Message Type.
   * @tparam  MSG_TYPE     Type for which new subscriber will be added.
   * @param   subscriber   Callable target.
   * @return  Handle associated with a registered subscriber. Use IsValidHandle()
   *          for operation success checking.
   *
    */
//   template <typename MSG_TYPE>
//   SubscriberHandle subscribe(std::function<void(MSG_TYPE&&)> subscriber) {
//     printf("Subscribe as MSG_TYPE&&\n");
//     return msgBusRepository.subscribe<MSG_TYPE&>(subscriber);
//   }

   template <typename MSG_TYPE>
   SubscriberHandle subscribe(Subscriber<MSG_TYPE> subscriber) {
     printf("Subscribe as MSG_TYPE\n");
     return msgBusRepository.subscribe<MSG_TYPE>(subscriber);
   }

   template <typename MSG_TYPE>
   SubscriberHandle subscribe(std::function<void(MSG_TYPE&)> subscriber) {
     printf("Subscribe as MSG_TYPE&\n");
     return msgBusRepository.subscribe<MSG_TYPE&>(subscriber);
   }


//
//  /*!***************************************************************************
//   * @brief   Subscribe for receiving messages of the specific Message Type.
//   *
//   * @tparam  MSG_TYPE     Type for which new subscriber will be added.
//   *
//   * @param   subscriber   Callable target.
//   *
//   * @return  Handle associated with a registered subscriber. Use IsValidHandle()
//   *          for operation success checking.
//   *
//   *****************************************************************************/
//  template <typename MSG_TYPE>
//  SubscriberHandle subscribe(Subscriber<MSG_TYPE> subscriber) {
//    return msgBusRepository.subscribe<MSG_TYPE>(subscriber);
//  }


  /*!***************************************************************************
   * @brief   UnSubscribe from receiving messages of the specific Message Type.
   *
   * @param   handle      Subscriber handle.
   *
   *****************************************************************************/
  void unSubscribe(SubscriberHandle& handle) {
    msgBusRepository.unSubscribe(handle);
  }

  /*!***************************************************************************
   * @brief   Publish message by blocking call. The method will return only
   *          when all subscribers  will receive published message.
   *
   * @tparam  MSG_TYPE    Message type - optional, will be deducted by compiler.
   *
   * @param   msg         Message to be published.
   *
   *****************************************************************************/
  template <typename MSG_TYPE>
  void publish(const MSG_TYPE& msg) {
    msgBusRepository.publish(msg);
  }

  bool isValidHandle(SubscriberHandle& handle) {
    return handle.isValid();
  }

  static MessageBus &instance() {
    static MessageBus _instance;
    return _instance;
  }

 private:
  MsgBusRepository msgBusRepository;
};
