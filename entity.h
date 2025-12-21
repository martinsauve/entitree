#ifndef ENTITY_H
#define ENTITY_H
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

typedef enum entityType {
   ENTITY_NONE = 0,
   ENTITY_PLAYER,
   ENTITY_ENEMY,
   ENTITY_BULLET,
   entitytype_size
} EntityType;

typedef struct entity {
   EntityType  type;
   Vector2     position;
   Vector2     size;
   Color       color;
   Vector2     speed;

   struct entity  *parent;
   size_t         child_count;
   size_t         child_capacity;
   struct entity  *childs;

   int (*draw_self)(struct entity *self);

} Entity;


void entity_childs_resize(Entity *self, int new_capacity);
int  entity_childs_append(Entity *self, Entity child);
int  entity_childs_count(Entity *self);
bool entity_is_ancestor(Entity *self, Entity *possible_ancestor);
bool entity_is_descendant(Entity *self, Entity *possible_descendant);
bool entity_is_root(Entity *self);

Entity *entity_childs_get(Entity *self, size_t index);
Entity *entity_get_parent(Entity *self);
Entity *entity_get_root(Entity *self);

Entity *entity_create(EntityType type, Vector2 position, Vector2 size, Color color, size_t child_capacity);
void free_entity(Entity *self);


#endif // ENTITY_H
