#include <stdio.h>
#include "entity.h"
#include "../third_party/uuid4/src/uuid4.h"
// TODO: compile third_party/uuid4/src/uuid4.c

void draw_player(Entity *player) {
   printf("drawing player %s", player->name);
}

Vector2 nullVec = {NAN, NAN};

int test_uuid(void)
{
   char buf[UUID4_LEN];

   uuid4_init();
   uuid4_generate(buf);
   printf("%s\n", buf);
   return EXIT_SUCCESS;
}

int test_entity(void)
{
   Entity root = entity_create(
         ENTITY_ROOT,
         "RootEntity",
         (Vector2){50, 50},         // position
         (Vector2){10, 10},         // size
         RED,                       // color
         0                        // child capacity
         );

   entity_add_children(&root, entity_create(
            ENTITY_PLAYER,
            "title",
            (Vector2){50, 50},         // position
            (Vector2){10, 10},     // size
            BLUE,                     // color
            0                        // child capacity
            ));

   Entity childofchild = entity_create(
         ENTITY_BULLET,
         "recurse",
         nullVec,
         nullVec,
         BLUE,                     // color
         0                        // child capacity
         );

   //child.draw_self = draw_player;
   entity_add_children(entity_get_child(&root, 0), childofchild);


   printf("Root type: %d\n", root.type);
   printf("Root has %d children\n", entity_recursive_count_children(&root, 12));

   if (entity_is_root(&root)) {
      printf("Entity root is root.\n");
   } else {
      printf("Entity root is not root.\n");
   }

   for (int i = 0; i < entity_count_children(&root); ++i)
   {
      Entity* e = entity_get_child(&root, i);
      printf("%s\n", e->name);
   }

   typedef enum {
      ZERO,
      ONE,
      TWO
   } Numbers;

   if (ENTITY_NONE == ZERO) {
      printf("ENTITY_NONE is zero.\n");
   }
   entity_destroy_children(&root);
   return 0;
}

int main(void)
{
   //test_entity();
   test_uuid();
}
