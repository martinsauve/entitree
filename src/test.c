#include <stdio.h>
#include "entity.h"
#include <assert.h>
#include "entity_json.h"
#include <raylib.h>



void draw_player(Entity *player) {
   printf("drawing player %s", player->name);
}

void print_entity(Entity *entity, int depth) {
   for (int i = 0; i < depth; ++i) printf(" | "); // indent based on depth
   printf("%s  :  %s\n", entity->name, entity->uuid);
}

Vector2 nullVec = {NAN, NAN};
EntityPos nullPos = {{NAN, NAN, NAN}};
EntitySize nullSize = {{NAN, NAN, NAN}};


int test_entity(void)
{
   Entity root = entity_create(
         ENTITY_2D_OBJECT,
         "RootEntity",
         (EntityPos){{50, 50}},         // position
         (EntitySize){{10, 10}},         // size
         RED,                       // color
         0                        // child capacity
         );

   entity_add_children(&root, entity_create(
            ENTITY_2D_OBJECT,
            "title",
            (EntityPos){{50, 50}},         // position
            (EntitySize){{10, 10}},     // size
            BLUE,                     // color
            0                        // child capacity
            ));

   Entity childofchild = entity_create(
         ENTITY_3D_OBJECT,
         "recurse",
         nullPos,
         nullSize,
         BLUE,                     // color
         0                        // child capacity
         );

   entity_add_children(entity_get_child(&root, 0), childofchild);
   entity_foreach_children_recursive(&root, 0, print_entity);
   //assert(root.children == NULL);
   //printf("-----------------\n");
   //entity_foreach_children_recursive(&root, 0, print_entity);
   printf("-----------------\n");
   dump_entity_tree_json(&root);
   debug_print_entity_flags_table(root.flags);
   entity_destroy_children(&root);
   return EXIT_SUCCESS;
}

int main(void)
{
   test_entity();
}
