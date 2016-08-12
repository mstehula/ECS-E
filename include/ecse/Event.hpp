#ifndef ECSE_EVENT_HPP
#define ECSE_EVENT_HPP

#include <vector>
#include <set>
#include <functional>
#include <unordered_map>

namespace ecse
{

  typedef int Identifier;

  class EventManager;

  class BaseEvent {};

  template <typename E>
  class Event : BaseEvent
  {
    friend class EventManager;
  };

  template <typename E>
  class EventSubscriberManager
  {
  public:
    template <typename L>
    void Subscribe(L* object, std::function<void()> fptr)
    {

    }

  private:
    std::set<std::function<void()>> function_pointers_;
  };

  class EventManager
  {
  public:
    EventManager() = default;
    virtual ~EventManager() {}

    template <typename E, typename L>
    void Subscribe(E event_type, L* listener, std::function<void()> fptr)
    {

    }

    template <typename E, typename L>
    void unsubscribe(E event_type, L listener, std::function<void()> fptr)
    {

    }

    template <typename E>
    void fire(E event)
    {

    }

  private:
     std::unordered_map<Identifier, EventSubscriberManager<BaseEvent>>  event_map_;
  };
}

#endif //ECSE_EVENT_HPP
