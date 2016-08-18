#ifndef ECSE_EVENT_HPP
#define ECSE_EVENT_HPP

#include <memory>
#include <functional>
#include <unordered_map>

namespace ecse
{
  struct Event {};

  class EventSubscriptionManagerBase {};

  template <class E>
  class EventSubscriptionManager : public EventSubscriptionManagerBase
  {
  public:
    void Subscribe(std::function<void(E)>  fptr)
    {
      fptr_ = fptr;
    }

    void Fire(E event)
    {
      fptr_(event);
    }
  private:
    std::function<void(E)> fptr_;
  };

  class EventManager
  {
  public:

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
      printf("Subscribed!\n");
    }

    template <class E>
    void Fire(E event)
    {
      std::shared_ptr<EventSubscriptionManager<E>> sub_manager_ptr;
      auto sub_manager_it = sub_managers_.find(typeid(E).hash_code());
      if(sub_manager_it == sub_managers_.end())
      {
          printf("There is not a subscription manager for this event. consider making one?\n");
      }
      else
      {
          sub_manager_ptr = std::static_pointer_cast<EventSubscriptionManager<E>>(sub_manager_it->second);
      }
      printf("Firing!\n");
      sub_manager_ptr->Fire(event);
      printf("Fired!\n");
    }

  private:
    std::unordered_map<std::uint32_t, std::shared_ptr<EventSubscriptionManagerBase>> sub_managers_;
  };
}

#endif //ECSE_EVENT_HPP
