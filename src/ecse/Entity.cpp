
#include <cstdio>

#include "ecse/Entity.hpp"

namespace ecse
{

  /*
   * Creates a new Entity and registers it to a slot with a good version number
   *
   * The implementation uses a free list to fill spaces that were vacated from
   *   prior versions
   *
   * Args: None
   * Return: The newly created Entity
   */
  Entity EntityManager::Create()
  {
    printf("Entity_Version_ vector: %p\n", entity_version_);

    uint32_t index, version;
    if(free_list_.empty())
    {
      version = 1;
      entity_version_.push_back(version);
    }
    else
    {
      index = free_list_.back();
      free_list_.pop_back();
      version = entity_version_.at(index);
    }
    Entity entity(this, Entity::Id(index, version));
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
  bool EntityManager::Validate(Entity::Id id)
  {
    return id.Index() < entity_version_.size() && entity_version_[id.Index()] == id.Version();
  }

  /*
   * Deletes the Entity provided
   *
   * Args: Entity Id of the Entity to be destroyed.
   * Return: None
   */
  void EntityManager::Destroy(Entity::Id id)
  {
    assert_valid(id);
    uint32_t index = id.Index();
    entity_version_[index]++;
    free_list_.push_back(index);
    return;
  }
}