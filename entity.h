#ifndef ENTITY_H
#define ENTITY_H
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

#define CHILDREN_DEFAULT_CAPACITY 64

typedef enum entityType {
   ENTITY_NONE = 0,
   ENTITY_ROOT,
   ENTITY_PLAYER,
   ENTITY_ENEMY,
   ENTITY_BULLET,
   ENTITY_TYPE_SIZE
} EntityType;

typedef struct entity {
   EntityType  type;
   Vector2     position;
   Vector2     size;
   Color       color;
   Vector2     speed;
   char        name[256];

   struct entity  *parent;

   size_t         children_count;
   size_t         children_capacity;
   struct entity  *children;

   void (*draw_func)(struct entity *entity);

} Entity;


// Resize the children array of an entity
void  entity_children_resize(Entity *entity, size_t new_capacity);

// Add a child (passed by value)
int   entity_add_children(Entity *parent, Entity child);
int   entity_count_children(Entity *parent);
int   entity_recursive_count_children(Entity *parent, int depth_limit);

bool  entity_is_root(Entity *entity);
bool  entity_is_ancestor_of(Entity *possible_ancestor, Entity *entity);
bool  entity_is_descendant_of(Entity *possible_descendant, Entity *entity);

Entity *entity_get_child(Entity *entity, size_t index);
Entity *entity_get_parent(Entity *entity);
Entity *entity_get_root(Entity *entity);

Entity entity_create(EntityType type, const char* name, Vector2 position, Vector2 size, Color color, size_t children_capacity);
void entity_destroy_children(Entity *entity);


#endif // ENTITY_H
