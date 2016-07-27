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

  /*
   * Forward declaration to use in BaseSystem and System.
   *
   * See also: SystemManager
   */
  class SystemManager;

  /*
   * BaseSystem should be an internal-use only. All external uses should be
   *   derived from the class System.
   *
   * BaseSystem gives a springboard point for the System class to used, without
   *   being too tied down with templating. The BaseSystem is also the class
   *   that is being stored in the map, as all Systems that are drived from
   *   System are guaranteed to be derived from BaseSystem as well.
   */
  class BaseSystem
  {
  public:

    /*
     * uint32_t Identifier is the stored type value for the template in System
     */
    typedef uint32_t Identifier;

    /*
     * Removing copy constructors to disallow copying of Systems.
     */
    BaseSystem(const BaseSystem&) = delete;
    BaseSystem& operator = (const BaseSystem &) = delete;

    /*
     * Default deconstructor
     */
    virtual ~BaseSystem() = default;

    /*
     * Static variable Identifier counter. This plays some magic in selecting
     *   the Identifier number for the System to use.
     */
    static Identifier id_counter_;
  };


  /*
   * The System class is one of the three staple classes for an ECS
   *
   * The System class should do a lot of the heavy lifting when it comes to
   *   coding logic. This class gives a template to include, store, acces and
   *   run systems in the ECS. All components should eventually be stored per
   *   system.
   */
  template <typename S>
  class System : BaseSystem
  {
  public:

    /*
     * Allows the System to be configured as they see fit. Must be called before
     *   the update loop begins.
     */
    virtual void configure() = 0;

    /*
     * Allows the System to be updated through the loops.
     */
    virtual void update(uint32_t dt) = 0;

  private:
    friend class SystemManager;

    /*
     * Gets the Identifier value for the specific System template
     */
    static Identifier ID()
    {
      static Identifier id = id_counter_++;
      return id;
    }
  };

  class SystemManager
  {
  public:

    /*
     * Adds the specified System to the SystemManager
     *
     * Template: S - the System name.
     *           Args - The arguments passed to the System's constructor
     * Args: Arguments are passed through the template.
     * Return: The shared pointer of the newly created system.
     */
    template <typename S, typename ... Args>
    std::shared_ptr<S> add(Args && ... args)
    {
      std::shared_ptr<S> system(new S(std::forward<Args>(args) ...));
      systems_.insert(std::make_pair(S::ID(), system));
      return system;
    }

    /*
     * Retrieves the System specified by the template argument
     *
     * Template: S - The System name to retrieve
     * Args: None
     * Return: The shared pointer to the retrieved system
     */
    template <typename S>
    std::shared_ptr<S> system()
    {
      auto pair = systems_.find(S::ID());
      return pair == systems_.end()
        ? std::shared_ptr<S>()
        : std::shared_ptr<S>(std::static_pointer_cast<S>(pair->second));
    }

  private:
    /*
     * The map is keyed by System::Identifier and value is the shared_ptr to
     *   the System.
     */
    std::unordered_map<BaseSystem::Identifier, std::shared_ptr<BaseSystem>> systems_;
  };

}

#endif //ECSE_SYSTEM_HPP
