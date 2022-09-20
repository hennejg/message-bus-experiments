/*
 * Based on code by Evgeny Zavalkovsky,
 * published under the The Code Project Open License (CPOL) 1.02
 * at https://www.codeproject.com/Articles/723656/SW-Message-Bus
 */

#pragma once

#include <condition_variable>
#include <mutex>

class Semaphore {
 public:
  explicit Semaphore(unsigned int initCount) : count(initCount) {
  }

  ~Semaphore() = default;

  /*!***************************************************************************
   * @brief   Lock the semaphore
   *          On positive counter returns immediately.
   *          On zero - blocked till countered increased.
   *****************************************************************************/
  void lock() {
    std::unique_lock<std::mutex> lock(mtx);
    cond.wait(lock, [this] { return (0 != count); });
    --count;
  }

  /*!***************************************************************************
   * @brief   UnLock the semaphore
   *          Increases semaphore count.
   *****************************************************************************/

  void unlock() {
    std::lock_guard<std::mutex> lock(mtx);
    ++count;
    cond.notify_one();
  }

 private:
  unsigned int count;
  std::condition_variable cond;
  std::mutex mtx;
};
