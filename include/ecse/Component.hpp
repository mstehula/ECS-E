#ifndef ECSE_COMPONENT_HPP
#define ECSE_COMPONENT_HPP

#include "ecse/Entity.hpp"

namespace ecse
{

  struct BaseComponent
  {
    typedef uint32_t Identifier;

    BaseComponent(const BaseComponent&) = delete;
    BaseComponent& operator = (const BaseComponent &) = delete;

    virtual ~BaseSystem() = default;

    static Identifier id_counter_;
  };

  template <typename C>
  struct Component : BaseComponent
  {

  };

  class ComponentManager
  {
  public:
    std::shared_ptr<Component> Create()
    {

    }

    bool Validate(Component::Identifier id)
    {

    }

    std::shared_ptr Get(Component::Identifier id)
    {

    }

    void Delete(Component::Identifier id)
    {

    }

    Component GetComponent(Entity::Identifier id)
    {

    }
  };

}

#endif //ECSE_COMPONENT_HPP
