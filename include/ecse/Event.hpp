#ifndef ECSE_EVENT_HPP
#define ECSE_EVENT_HPP

#include <vector>
#include <unordered_map>

namespace ecse
{

  class EventManager;

  class BaseEvent
  {
  public:
    typedef uint32_t Identifier;

    virtual ~BaseEvent() = default;
  protected:
    static Identifier id_counter_;
  };

  template <typename E>
  class Event : BaseEvent
  {
    /*
     * Gets the Identifier value for the specific System template
     */
    static Identifier ID()
    {
      static Identifier id = id_counter_++;
      return id;
    }
  };

  class EventManager
  {
  public:
    EventManager() = default;
    virtual ~EventManager()
    {

    }

    template <typename L>
    class EventLink
    {
      typedef void (L::*fptr)(void);
    public:
      EventLink(L *object, fptr function) : object_(object), function_(function) {}

    private:
      L* object_;
      fptr function_;
    };

    template <typename E, typename L>
    void subscribe(E event_type, L listener, void (L::*fptr)(void))
    {

    }

    template <typename E, typename L>
    void unsubscribe(E event_type, L listener, void (L::*fptr)(void))
    {

    }

    template <typename E>
    void fire(E event)
    {

    }

  private:
     std::unordered_map<BaseEvent::Identifier, std::vector<EventLink<BaseEvent>>>  event_map_;

  };
}

#endif //ECSE_EVENT_HPP
