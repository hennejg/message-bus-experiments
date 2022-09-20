/*
 * Based on code by Evgeny Zavalkovsky,
 * published under the The Code Project Open License (CPOL) 1.02
 * at https://www.codeproject.com/Articles/723656/SW-Message-Bus
 */

#pragma once

#include <cstdint>

using TypeId = uintptr_t;

template <typename T>
static TypeId getTypeId() {
  return (reinterpret_cast<TypeId>(typeid(T).name()));
}
