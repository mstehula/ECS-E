#include "ecse/Entity.hpp"

#include <cstdio>

int main(int argc, char** argv)
{
  printf("EM: \t%d\n",sizeof(ecse::EntityManager));
  printf("EId: \t%d\n",sizeof(ecse::Entity::Id));
  printf("E: \t%d\n",sizeof(ecse::Entity));
}

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
    uint32_t index, version;
    if(free_list_.empty())
    {
      index = index_counter_++;
      version = entity_version_[index] = 1;
    }
    else
    {
      index = free_list_.back();
      free_list_.pop_back();
      version = entity_version_[index];
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
  bool EntityManager::Valid(Entity::Id id)
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
