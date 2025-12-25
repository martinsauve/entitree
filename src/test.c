#include <stdio.h>
#include "entity.h"
#include <assert.h>
#include "../third_party/uuid4/src/uuid4.h"
#include "../third_party/cJSON/cJSON.h"

cJSON *entity_to_json(Entity *entity) {
   cJSON *json = cJSON_CreateObject();
   cJSON_AddStringToObject(json, "uuid", entity->uuid);
   cJSON_AddStringToObject(json, "name", entity->name);
   cJSON_AddNumberToObject(json, "x", entity->position.x);
   cJSON_AddNumberToObject(json, "y", entity->position.y);
   cJSON_AddNumberToObject(json, "width", entity->size.x);
   cJSON_AddNumberToObject(json, "height", entity->size.y);

   if (entity->children_count > 0) {
      cJSON *childrenjson = cJSON_CreateArray();
      for (size_t i = 0; i < entity->children_count; ++i) {
         cJSON_AddItemToArray(childrenjson, entity_to_json(&entity->children[i]));
      }
      cJSON_AddItemToObject(json, "children", childrenjson);
   }
   return json;
}

void dump_entity_tree_json(Entity *root)
{
   cJSON *json = entity_to_json(root);
   char *string = cJSON_Print(json);
   printf("%s\n", string);
}


void draw_player(Entity *player) {
   printf("drawing player %s", player->name);
}

void print_entity(Entity *entity, int depth) {
   for (int i = 0; i < depth; ++i) printf(" | "); // indent based on depth
   printf("%s  :  %s\n", entity->name, entity->uuid);
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

   entity_add_children(entity_get_child(&root, 0), childofchild);
   entity_foreach_children_recursive(&root, 0, print_entity);
   //entity_destroy_children(&root);
   //assert(root.children == NULL);
   //printf("-----------------\n");
   //entity_foreach_children_recursive(&root, 0, print_entity);
   printf("-----------------\n");
   dump_entity_tree_json(&root);
   return EXIT_SUCCESS;
}

int main(void)
{
   test_entity();
   //test_uuid();
}
