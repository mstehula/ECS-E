#ifndef ECSE_EVENT_HPP
#define ECSE_EVENT_HPP

#include <vector>
#include <set>
#include <functional>
#include <unordered_map>

namespace ecse
{

  typedef int Identifier;

  template <class E>
  class EventManager;

  template <typename E>
  class Event
  {
  };

  template <class E>
  class EventSubscriptionManager
  {
  public:
    void Subscribe(std::function<void(E)>  fptr)
    {
      event_function_ = fptr;
    }

    void Fire(E event)
    {
      event_function_(event);
    }
  private:
    std::function<void(E)> event_function_;
  };


  template <class E>
  class EventManager
  {
  public:
    void Subscribe(std::function<void(E)> fptr)
    {
      sub_manager_.Subscribe(fptr);
    }

    void Fire(E event)
    {
      sub_manager_.Fire(event);
    }

  private:
    EventSubscriptionManager<E> sub_manager_;
  };
}

#endif //ECSE_EVENT_HPP
