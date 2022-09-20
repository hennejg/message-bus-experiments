/*
 * Based on code by Evgeny Zavalkovsky,
 * published under the The Code Project Open License (CPOL) 1.02
 * at https://www.codeproject.com/Articles/723656/SW-Message-Bus
 */

#pragma once

#include <functional>

template <typename MSG_TYPE> using Subscriber = std::function<void(const MSG_TYPE&)>;
