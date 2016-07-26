#ifndef ECS_ENTITY_HPP
#define ECS_ENTITY_HPP

#include <cstdint>
#include <cassert>
#include <vector>

namespace ecse
{

  class EntityManager;

  /*
   * The Entity class is one of the three staple classes for an ECS
   *
   * The Entity class should do little more than give something for components
   *   to "have a home" per say, give it an ID to latch onto. This uses an ID
   *   of 64 bits, 32 being an index and 32 being a version. There are up to
   *   ~4 billion different Entity's that we can use, with ~4 billion versions
   *   before we start repeating GUID's. We may give the ability to edit this to
   *   move the bit count around to allow less Entities and more versions.
   */
  class Entity
  {
  public:
    struct Id
    {
    public:

      /*
       * Base constructor, constructs Entity with Id = 0
       * Explicit constructor, constructs Entity with Id = id
       * Implicit constructor, constructs Entity with Id = index | version << 32
       */
      Id() : id_(0) {}
      explicit Id(uint64_t id) : id_(id) {}
      Id(uint32_t index, uint32_t version) : id_(uint64_t(index) | uint64_t(version) << 32UL) {}

      /*
       * Returns full 64 bit Id
       * Returns first 32 bits of the Id
       * Returns last 32 bist of the Id, shifted
       */
      uint64_t Get() const { return id_; }
      uint32_t Index() const { return id_ & 0xffffffffUL; }
      uint32_t Version() const { return id_ >> 32; }

      /*
       * Compares the Id for boolean operations
       * Compares the Id for boolean operations
       */
      bool operator == (const Id &other) const { return id_ == other.id_; }
      bool operator != (const Id &other) const { return id_ != other.id_; }

    private:

      /*
       * This is the definition so that the Entity can access certain aspects
       *   of the EntityManager that is controlling it
       */
      friend class EntityManager;

      /*
       * Private storage for the Id, the only data that is really stored here
       */
      uint64_t id_;
    };

    /*
     * Constant value for the Id to be when it is INVAILD
     */
    static const Id INVALID;

    /*
     * Base constructor, defaulted
     * Implicit constructor, builds from an EntityManager pointer and an Id
     * Implicit constructor, copy constructor
     * Implicit constructor, copy constructor
     */
    Entity() = default;
    Entity(EntityManager *manager, Entity::Id id) : manager_(manager), id_(id) {}
    Entity(const Entity &other) = default;
    Entity &operator = (const Entity &other) = default;

    /*
     * Operator comparison, compares when compared with nothing
     *   ie. if(Entity) ==> if(Entity.valid())
     * Operator comparison to compare entities, if manager and Id are the same
     * Operator comparison to compare entities, if manager and Id are not the same
     */
    operator bool() const { id_ != INVALID; }

    bool operator == (const Entity &other) const
    {
      return other.manager_ == manager_ && other.id_ == id_;
    }

    bool operator != (const Entity &other) const
    {
      return other.manager_ != manager_ && other.id_ == id_;
    }

  private:
    /*
     * The EntityManager pointer that the Entity is currently registered to
     */
    EntityManager *manager_ = nullptr;

    /*
     * The current Id of the Entity
     */
    Entity::Id id_ = INVALID;
  };

  /*
   * The EntityManager is the class that handles many of the creation and
   *   destruction of Entities. There should only be 1 EntityManager for
   *   every piece of data, ie. "world" or "server"
   */
  class EntityManager
  {
  public:

    /*
     * Base constructor, defaulted
     * Base destructor, empty
     */
    EntityManager() = default;
    virtual ~EntityManager() {}

    /*
     * Create()
     * Args: None
     * Returns: Newly created Entity
     */
    Entity Create();

    /*
     * Valid()
     * Args: the Entity Id of the Entity to check
     * Return: boolean weather the Entity is valid
     */
    bool Valid(Entity::Id id);

    /*
     * Destroy()
     * Args: the Entity Id of which to destroy
     * Return: None
     */
    void Destroy(Entity::Id);

    /*
     * Inline funtion to assert we are not using a stale Entity ID
     */
     inline void assert_valid(Entity::Id id) const
     {
       assert(id.Index() < entity_version_.size() && "Entity::Id outside entity range");
       assert(entity_version_[id.Index()] == id.Version() && "Attempted to access stale Entity");
     }

     friend class Entity;
   private:

    /*
     * Holds the number of indices currently in use. Only has the ability to go
     * up, not down.
     */
    uint32_t index_counter_ = 0;

    /*
     * Vector of Entity versions for each Entity. Increases on Entity
     *   destruction, rolls over when it hits the max version number. Versions
     *   are what we determine when we are asking if an Entity is "valid"
     */
    std::vector<uint32_t> entity_version_;

    /*
     * The Free List is a piece of data that is used to organize indices of data
     *   that has been deleted from the middle of arrays, using the indices of
     *   deleted or invalid Entities before expanding the array to use more
     */
    std::vector<uint32_t> free_list_;
  };
}

#endif //ECS_ENTITY_HPP
