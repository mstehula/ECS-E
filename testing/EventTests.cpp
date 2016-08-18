
#include <cstdio>
#include <functional>

#include "ecse/Event.hpp"

struct TestEvent : public ecse::Event
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
  ecse::EventManager event_manager;
  Listener listener_;
  TestEvent event;

  event_manager.Subscribe<TestEvent>(listener_.Callback);
  printf("%s\n", "Callback registered!");
  event_manager.Fire(event);
  printf("%s\n", "Callback complete!");
}
