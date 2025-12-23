#include <stdio.h>
#include "entity.h"

void draw_player(Entity *player) {
   printf("drawing player %s", player->name);
}

int main(void)
{
   // TODO: entity_create should return a struct, not a pointer
   Entity root = entity_create(
         ENTITY_ROOT,
         "RootEntity",
         (Vector2){50, 50},         // position
         (Vector2){10, 10},         // size
         RED,                       // color
         0                        // child capacity
         );

   Entity child = entity_create(
         ENTITY_PLAYER,
         "title",
         (Vector2){50, 50},         // position
         (Vector2){10, 10},     // size
         BLUE,                     // color
         0                        // child capacity
         );
   Entity childofchild = entity_create(
         ENTITY_PLAYER,
         "recurse",
         (Vector2){50, 50},         // position
         (Vector2){10, 10},     // size
         BLUE,                     // color
         0                        // child capacity
         );

   child.draw_self = draw_player;
   entity_childs_append(&root, child);
   entity_childs_append(entity_childs_get(&root, 0), childofchild);


   printf("Root type: %d\n", root.type);
   printf("Root has %d childrens\n", entity_childs_recursive_count(&root, 12));

   if (entity_is_root(&root)) {
      printf("Entity root is root.\n");
   } else {
      printf("Entity root is not root.\n");
   }

   for (int i = 0; i < entity_childs_count(&root); ++i)
   {
      Entity* e = entity_childs_get(&root, i);
      printf("%s\n", e->name);
   }

   free_entity(&root);
   return 0;
}
