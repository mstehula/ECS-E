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
    struct EventLink
    {
      typedef void (L::_fptr)(void);
    public:
      EventLink(L *object, _fptr function) : object_(object), fptr_(function_) {}

    private:
      L* object_;
      _fptr function_;
    };

    template <typename E>
    void subscribe(E event_type, L listener, fptr function)
    {

    }

    template <typename E>
    void unsubscribe(E event_type, L listener, fptr function)
    {

    }

    template <typename E>
    void fire(E event)
    {

    }

  private:
     unordered_map<BaseEvent::Identifier, Vector<EventLink>>  event_map_;

  };
}

#endif //ECSE_EVENT_HPP
