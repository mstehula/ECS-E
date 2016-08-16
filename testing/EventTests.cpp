
#include <cstdio>
#include <functional>

#include "ecse/Event.hpp"

struct TestEvent : ecse::Event<TestEvent>
{

};

class Listener
{
public:

  static void Callback(TestEvent event)
  {
    printf("Callback is here!\n");
  }

};

int main()
{
  ecse::EventManager<TestEvent> event_manager;
  Listener listener_;
  TestEvent event;

  event_manager.Subscribe(listener_.Callback);
  printf("%s\n", "Callback registered!");
  event_manager.Fire(event);
  printf("%s\n", "Callback complete!");
}
