
#include <cstdio>

#include <ecse/Entity.hpp>

int main(int argc, char** argv)
{
  printf("EM: \t%d\n",sizeof(ecse::EntityManager));
  printf("EId: \t%d\n",sizeof(ecse::Entity::Identifier));
  printf("E: \t%d\n",sizeof(ecse::Entity));

  ecse::EntityManager em;

  ecse::Entity entity1 = em.Create();
  ecse::Entity entity2 = em.Create();

  printf("E1 %d\n", entity1.ID().ID());
  printf("E1,I %d\n", entity1.ID().Index());
  printf("E1,V %d\n", entity1.ID().Version());
  printf("E2 %d\n", entity2.ID().ID());
  printf("E2,I %d\n", entity2.ID().Index());
  printf("E2,V %d\n", entity2.ID().Version());

  printf("E1 valid: %d\n", em.Validate(entity1.ID()));
  printf("E2 valid: %d\n", em.Validate(entity2.ID()));

  em.Destroy(entity1.ID());
  em.Destroy(entity2.ID());

  printf("E1 valid: %d\n", em.Validate(entity1.ID()));
  printf("E2 valid: %d\n", em.Validate(entity2.ID()));

  entity1 = em.Create();
  printf("E1 %d\n", entity1.ID().ID());
  printf("E1,I %d\n", entity1.ID().Index());
  printf("E1,V %d\n", entity1.ID().Version());

}
