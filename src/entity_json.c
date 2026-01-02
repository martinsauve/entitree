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
   cJSON_AddNumberToObject(json, "x", entity->pos.as_vec2.x);
   cJSON_AddNumberToObject(json, "y", entity->pos.as_vec2.y);
   if (entity->flags & ENTITY_IS_3D) cJSON_AddNumberToObject(json, "z", entity->pos.as_vec3.z);

   cJSON_AddNumberToObject(json, "width", entity->size.as_vec2.x);
   cJSON_AddNumberToObject(json, "height", entity->size.as_vec2.y);
   if (entity->flags & ENTITY_IS_3D) cJSON_AddNumberToObject(json, "depth", entity->size.as_vec3.z);

   cJSON_AddNumberToObject(json, "speed_x", entity->speed.as_vec2.x);
   cJSON_AddNumberToObject(json, "speed_y", entity->speed.as_vec2.y);
   if (entity->flags & ENTITY_IS_3D) cJSON_AddNumberToObject(json, "speed_z", entity->speed.as_vec3.z);

   cJSON_AddNumberToObject(json, "color", ColorToInt(entity->color));
   if (entity->children_count > 0) {
      cJSON *childrenJson = cJSON_CreateArray();
      for (size_t i = 0; i < entity->children_count; ++i) {
         cJSON_AddItemToArray(childrenJson, entity_to_json(&entity->children[i]));
      }
      cJSON_AddItemToObject(json, "children", childrenJson);
   }
   return json;
}

void entity_dump_tree_json(Entity *root)
{
   cJSON *json = entity_to_json(root);
   char *string = cJSON_Print(json);
   printf("%s\n", string);
   free(string);
   cJSON_Delete(json);
}
