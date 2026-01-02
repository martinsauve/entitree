#ifndef ENTITY_JSON_H
#define ENTITY_JSON_H
#include "entity.h"
#include "../third_party/cJSON/cJSON.h"


cJSON *entity_to_json(Entity *entity);
void entity_dump_tree_json(Entity *root);

#endif //ENTITY_JSON_H
