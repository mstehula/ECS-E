#ifndef ECSE_EVENT_HPP
#define ECSE_EVENT_HPP

#include <memory>
#include <functional>
#include <unordered_map>
#include <vector>

namespace ecse
{

  /*
   * Base event structure. All events should extend this class. All internal
   *   members should be of a POD type to keep everything seperate
   */
  struct Event {};

  /*
   * The EventSubscriptionManager base, to allow for polymorphic pointer access
   *   using std::static_pointer_cast();
   */
  class EventSubscriptionManagerBase {};

  /*
   * The EventSubscriptionManager.
   *
   * Has template for the Event Type that it is currently running with. This is
   *   passed in as template E
   *
   * void Subscribe(fptr)
   *   takes in the function pointer and stores it in a vector to call whenever
   *   the Event Type is fired.
   *
   * void Fire(event)
   *   takes in an event to pass along to all of the callback functions.
   *
   * private members:
   *   vector containing all std::function pointers
   */
  template <class E>
  class EventSubscriptionManager : public EventSubscriptionManagerBase
  {
  public:

    /*
     * void Subscribe(std::function<void(E)> fptr)
     *
     * Stores the callback function pointer that is passed in through the args
     *   in the vector fptrs_
     *
     * Args:
     *   std::function<void(E)> fptr
     *     the function pointer that is currently being subscribed
     */
    void Subscribe(std::function<void(E)>  fptr)
    {
      fptrs_.push_back(fptr);
    }

    /*
     * void Fire(E event)
     *
     * Takes all stored callback function pointers and fires them in no
     *   particular order
     *
     * Args:
     *   template<E> event
     *     the copy of the event that is being passed to all callback methods
     */
    void Fire(E event)
    {
      for(typename std::vector<std::function<void(E)>>::iterator it = fptrs_.begin(); it != fptrs_.end(); ++it)
      {
        (*it)(event);
      }
    }
  private:
    /*
     * Storage location for all of the callback member pointers
     */
    std::vector<std::function<void(E)>> fptrs_;
  };

  /*
   * The EventManager.
   *
   * With anything that needs to do with events, this is the go-to class to
   *  call from. Subscribe and Fire are the two functions currently implemented.
   *
   * void Subscribe(fptr)
   *   The method that will be called from outside the class to subscribe to an
   *   event.
   *
   * void Fire(event)
   *   The method that will be called from outside the class to fire an event,
   *   given by the argument. This must be a fully built event to fire.
   *
   * private members:
   *   sub_managers_
   *     sub_managers_ is the unordered_map that links the Event Type with the
   *     EventSubscriptionManager. All data for the EventSubscriptionManager is
   *     stored in a pointer so polymorphism can happen through a
   *     static_pointer_cast
   */
  class EventManager
  {
  public:

    /*
     * Subscribe
     *
     * Subscribes the function pointer to the Event Type passed through the
     *   template. Stores the EventSubscriptionManager in the unordered_map
     *
     *
     * Args:
     *   std::function<void(E)> fptr
     *     The function pointer of the callback method outside the class.
     *
     * Return: void
     */
    template <class E>
    void Subscribe(std::function<void(E)> fptr)
    {
      std::shared_ptr<EventSubscriptionManager<E>> sub_manager_ptr;
      auto sub_manager_it = sub_managers_.find(typeid(E).hash_code());
      if(sub_manager_it == sub_managers_.end())
      {
          sub_manager_ptr = std::make_shared<EventSubscriptionManager<E>>();
      }
      else
      {
          sub_manager_ptr = std::static_pointer_cast<EventSubscriptionManager<E>>(sub_manager_it->second);
      }
      sub_manager_ptr->Subscribe(fptr);
      sub_managers_.insert(std::pair<std::size_t, std::shared_ptr<EventSubscriptionManagerBase>>(typeid(E).hash_code(), sub_manager_ptr));
    }

    /*
     * Fire
     *
     * Fires the event that is passed into it through all of the callback
     *   functions that are registered to the Event Type passed through the
     *   template.
     *
     * Args:
     *   template <class E> event
     *     The fully instantiated event. This event is then passed on to the
     *     callback methods to pass data from one place to another.
     * Return: void
     */
    template <class E>
    void Fire(E event)
    {
      std::shared_ptr<EventSubscriptionManager<E>> sub_manager_ptr;
      auto sub_manager_it = sub_managers_.find(typeid(E).hash_code());
      if(sub_manager_it == sub_managers_.end())
      {
        return;
      }
      else
      {
          sub_manager_ptr = std::static_pointer_cast<EventSubscriptionManager<E>>(sub_manager_it->second);
      }
      sub_manager_ptr->Fire(event);
    }

  private:
    /*
     * Unordered_map that links Event Type, stored in a uint32_t, and the
     *   EventSubscriptionManagers, stored as shared_ptr to allow polymorphism
     */
    std::unordered_map<std::uint32_t, std::shared_ptr<EventSubscriptionManagerBase>> sub_managers_;
  };
}

#endif //ECSE_EVENT_HPP
