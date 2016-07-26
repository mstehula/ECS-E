
#include <cstdio>

#include <ecse/Entity.hpp>

int main(int argc, char** argv)
{
  printf("EM: \t%d\n",sizeof(ecse::EntityManager));
  printf("EId: \t%d\n",sizeof(ecse::Entity::Id));
  printf("E: \t%d\n",sizeof(ecse::Entity));
}
