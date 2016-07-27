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
    struct Identifier
    {
    public:

      /*
       * Base constructor, constructs Entity with Id = 0
       * Explicit constructor, constructs Entity with Id = id
       * Implicit constructor, constructs Entity with Id = index | version << 32
       */
      Identifier() : id_(0) {}
      explicit Identifier(uint64_t id) : id_(id) {}
      Identifier(uint32_t index, uint32_t version) : id_(uint64_t(index) | uint64_t(version) << 32UL) {}

      /*
       * Returns full 64 bit Id
       * Returns first 32 bits of the Id
       * Returns last 32 bist of the Id, shifted
       */
      uint64_t ID() const { return id_; }
      uint32_t Index() const { return id_ & 0xffffffffUL; }
      uint32_t Version() const { return id_ >> 32; }

      /*
       * Compares the Id for boolean operations
       * Compares the Id for boolean operations
       */
      bool operator == (const Identifier &other) const { return id_ == other.id_; }
      bool operator != (const Identifier &other) const { return id_ != other.id_; }

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
    static const Identifier INVALID;

    inline Identifier ID()
    {
      return id_;
    }

    /*
     * Base constructor, defaulted
     * Implicit constructor, builds from an EntityManager pointer and an Id
     * Implicit constructor, copy constructor
     * Implicit constructor, copy constructor
     */
    Entity() = default;
    Entity(EntityManager *manager, Entity::Identifier id) : manager_(manager), id_(id) {}
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
    Entity::Identifier id_ = INVALID;
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
     * Creates a new Entity and registers it to a slot with a good version number
     *
     * The implementation uses a free list to fill spaces that were vacated from
     *   prior versions
     *
     * Args: None
     * Return: The newly created Entity
     */
    Entity Create()
    {
      uint32_t index, version;
      if(free_list_.empty())
      {
        index = index_counter_++;
        version = 1;
        entity_version_.push_back(version);
      }
      else
      {
        index = free_list_.back();
        free_list_.pop_back();
        version = entity_version_.at(index);
      }
      Entity entity(this, Entity::Identifier(index, version));
      return entity;
    }

    /*
     * Checks validity of the Entity and returns a boolean.
     *
     * See also:
     *    assert_valid()
     *
     * Args: Entity Id of the Entity to validate
     * Return: Boolean weather it is valid or not
     */
    bool Validate(Entity::Identifier id)
    {
      return id.Index() < entity_version_.size() && entity_version_[id.Index()] == id.Version();
    }

    /*
     * Deletes the Entity provided
     *
     * Args: Entity Identifier of the Entity to be destroyed.
     * Return: None
     */
    void Destroy(Entity::Identifier id)
    {
      assert_valid(id);
      uint32_t index = id.Index();
      entity_version_[index]++;
      free_list_.push_back(index);
      return;
    }

    /*
     * Inline funtion to assert we are not using a stale Entity ID
     */
     inline void assert_valid(Entity::Identifier id) const
     {
       assert(id.Index() < entity_version_.size() && "Entity::Id outside entity range");
       assert(entity_version_[id.Index()] == id.Version() && "Attempted to access stale Entity");
     }

     friend class Entity;
   private:

    /*
     * Index for which is the next index to add when there are no places in the
     *   free list
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
