
#include <cstdio>

#include <ecse/Entity.hpp>

int main(int argc, char** argv)
{
  printf("EM: \t%d\n",sizeof(ecse::EntityManager));
  printf("EId: \t%d\n",sizeof(ecse::Entity::Id));
  printf("E: \t%d\n",sizeof(ecse::Entity));

  ecse::EntityManager em;

  ecse::Entity entity1 = em.Create();
  ecse::Entity entity2 = em.Create();

  printf("E1 %d\n", entity1.GetId().Get());
  printf("E1,I %d\n", entity1.GetId().Index());
  printf("E1,V %d\n", entity1.GetId().Version());
  printf("E2 %d\n", entity2.GetId().Get());
  printf("E2,I %d\n", entity2.GetId().Index());
  printf("E2,V %d\n", entity2.GetId().Version());

  printf("E1 valid: %d\n", em.Validate(entity1.GetId()));
  printf("E2 valid: %d\n", em.Validate(entity2.GetId()));

  em.Destroy(entity1.GetId());
  em.Destroy(entity2.GetId());

  printf("E1 valid: %d\n", em.Validate(entity1.GetId()));
  printf("E2 valid: %d\n", em.Validate(entity2.GetId()));

  entity1 = em.Create();
  printf("E1 %d\n", entity1.GetId().Get());
  printf("E1,I %d\n", entity1.GetId().Index());
  printf("E1,V %d\n", entity1.GetId().Version());
  
}
