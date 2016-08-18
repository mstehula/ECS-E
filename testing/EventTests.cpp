
#include <cstdio>
#include <functional>

#include "ecse/Event.hpp"

struct TestEvent : public ecse::Event
{

};

class Listener
{
public:

  static void Callback1(TestEvent event)
  {
    printf("Callback 1 is here!\n");
  }

  static void Callback2(TestEvent event)
  {
    printf("Callback 2 is here!\n");
  }

};

int main()
{
  ecse::EventManager event_manager;
  Listener listener_;
  TestEvent event;

  event_manager.Subscribe<TestEvent>(listener_.Callback1);
  event_manager.Subscribe<TestEvent>(listener_.Callback2);
  printf("%s\n", "Callbacks registered!");
  event_manager.Fire(event);
  printf("%s\n", "Callback complete!");
}
