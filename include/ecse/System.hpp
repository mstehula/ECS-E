#ifndef ECSE_SYSTEM_HPP
#define ECSE_SYSTEM_HPP

#include <cstdint>
#include <cassert>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "ecse/Entity.hpp"

namespace ecse
{

  class SystemManager;

  class BaseSystem
  {
  public:
    typedef uint32_t Identifier;

    BaseSystem(const BaseSystem&) = delete;
    BaseSystem& operator = (const BaseSystem &) = delete;

    virtual ~BaseSystem() {}

    static Identifier id_counter_;
  };

  template <typename S>
  class System : BaseSystem
  {
  public:
    virtual void configure();
    virtual void update(uint32_t dt) = 0;

  private:
    friend class SystemManager;

    static Identifier ID()
    {
      static Identifier id = id_counter_++;
      return id;
    }
  };

  class SystemManager
  {
  public:

    template <typename S, typename ... Args>
    std::shared_ptr<S> add(Args && ... args)
    {
      std::shared_ptr<S> system(new S(std::forward<Args>(args) ...));
      systems_.insert(std::make_pair(S::ID(), system));
      return system;
    }

    template <typename S>
    std::shared_ptr<S> system()
    {
      auto pair = systems_.find(S::ID());
      return pair == systems_.end()
        ? std::shared_ptr<S>()
        : std::shared_ptr<S>(std::static_pointer_cast<S>(pair->second));
    }

  private:
    std::unordered_map<BaseSystem::Identifier, std::shared_ptr<BaseSystem>> systems_;
  };

}


#endif //ECSE_SYSTEM_HPP
