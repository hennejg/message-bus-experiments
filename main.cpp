#include <iostream>
#include <memory>
#include <vector>

#include "messagebus/MessageBus.h"

struct Foo {
  Foo() { printf("  Foo\n"); }
  Foo(const Foo &f) { printf("  Foo (copied)\n"); }
  // Foo(const Foo &f) = delete;
  ~Foo() { printf("  ~Foo\n"); }
};

int main() {
  MessageBus::instance().subscribe<Foo>(
      [](Foo f) { printf("      Subscriber<Foo>(Foo f)\n"); });
  MessageBus::instance().subscribe<Foo>(
      [](Foo &f) { printf("      Subscriber<Foo>(Foo &f)\n"); });
  MessageBus::instance().subscribe<Foo &>(
      [](Foo &f) { printf("      Subscriber<Foo&>(Foo &f)\n"); });
  MessageBus::instance().subscribe<Foo &>(
      [](Foo f) { printf("      Subscriber<Foo&>(Foo f)\n"); });

  Foo foo{};

  printf("Publish 1\n");
  MessageBus::instance().publish(foo);

  return 0;
}