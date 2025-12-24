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

// Add a child (passed by value). The memory of children is managed by the parent entity. You can free it with `entity_destroy_children(parent)`.
int   entity_add_children(Entity *parent, Entity child);

// Count direct children (not recursive) of an entity.
// You can also use `entity_recursive_count_children()` for the recursive version.
int   entity_count_children(Entity *parent);

// Count all descendants (recursive) of an entity, up to a certain depth limit.
int   entity_recursive_count_children(Entity *parent, int depth_limit);

bool  entity_is_root(Entity *entity);

// Check if `possible_ancestor` is somewhere in the ancestry tree of `entity`
bool  entity_is_ancestor_of(Entity *possible_ancestor, Entity *entity);

// Check if `possible_descendant` is somewhere in the children tree of `entity`
bool  entity_is_descendant_of(Entity *possible_descendant, Entity *entity);

// Get a child entity by index
Entity *entity_get_child(Entity *entity, size_t index);

// Get the parent entity
Entity *entity_get_parent(Entity *entity);

// Get the base/root entity of the hierarchy (first entity without a parent in the tree)
Entity *entity_get_root(Entity *entity);

// Create a new entity with specified parameters (Entity is returned by value, so NOT allocated on the heap, the memory is managed by the caller.)
// If you use `entity_add_children()`, the memory will be managed by the parent entity, free it with `entity_destroy_children()`.
Entity entity_create(EntityType type, const char* name, Vector2 position, Vector2 size, Color color, size_t children_capacity);

// Destroy recursively all children of an entity and free their memory
void entity_destroy_children(Entity *entity);


#endif // ENTITY_H
