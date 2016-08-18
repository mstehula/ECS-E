
#include <cstdio>
#include <functional>

#include "ecse/Event.hpp"

struct TestEvent1 : public ecse::Event
{
public:
  int id;
};

struct TestEvent2 : public TestEvent1
{
public:
  int x, y, z;
};

class Listener
{
public:

  static void Callback1(TestEvent1 event)
  {
    printf("Callback 1 is here!\n");
    printf("%d\n", event.id);
  }

  static void Callback2(TestEvent2 event)
  {
    printf("Callback 2 is here!\n");
    printf("%d %d %d %d\n", event.id, event.x, event.y, event.z);
  }

};

int main()
{
  ecse::EventManager event_manager;
  Listener listener_;
  TestEvent1 event1;
  event1.id = 1;

  TestEvent2 event2;
  event2.id = 2;
  event2.x = 1;
  event2.y = 100;
  event2.z = 120;

  printf("Test Event 1\n");
  event_manager.Subscribe<TestEvent1>(listener_.Callback1);
  event_manager.Fire(event1);
  event_manager.Fire((TestEvent1) event2);

  printf("Test Event 2\n");
  event_manager.Subscribe<TestEvent2>(listener_.Callback2);
  event_manager.Fire(event2);
}
