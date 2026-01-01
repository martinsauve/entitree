#include "entity_json.h"
#include "entity.h"
#include <stdio.h>

cJSON *entity_to_json(Entity *entity) {
   char flags_str[256];
   debug_entity_flags_string(entity->flags, flags_str, sizeof(flags_str));
   cJSON *json = cJSON_CreateObject();
   cJSON_AddNumberToObject(json, "flags", entity->flags);
   cJSON_AddStringToObject(json, "flags_as_str", flags_str);
   cJSON_AddStringToObject(json, "uuid", entity->uuid);
   cJSON_AddStringToObject(json, "name", entity->name);
   cJSON_AddNumberToObject(json, "x", entity->pos.pos2d.x);
   cJSON_AddNumberToObject(json, "y", entity->pos.pos2d.y);
   if (entity->flags & ENTITY_IS_3D) cJSON_AddNumberToObject(json, "z", entity->pos.pos3d.z);

   cJSON_AddNumberToObject(json, "width", entity->size.size2d.x);
   cJSON_AddNumberToObject(json, "height", entity->size.size2d.y);
   if (entity->flags & ENTITY_IS_3D) cJSON_AddNumberToObject(json, "depth", entity->size.size3d.z);

   cJSON_AddNumberToObject(json, "speed_x", entity->speed.speed2d.x);
   cJSON_AddNumberToObject(json, "speed_y", entity->speed.speed2d.y);
   if (entity->flags & ENTITY_IS_3D) cJSON_AddNumberToObject(json, "speed_z", entity->speed.speed3d.z);

   cJSON_AddNumberToObject(json, "color", ColorToInt(entity->color));
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
   free(string);
   cJSON_Delete(json);
}
