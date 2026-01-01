#ifndef ENTITY_H
#define ENTITY_H
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../third_party/uuid4/src/uuid4.h"

#define CHILDREN_DEFAULT_CAPACITY 64

/// The types Vector2, Vector3 and Color are compatible with (and provided by) raylib. You can use <raymath.h> functions to manipulate them.

// TODO: thoses are copy pasted from raylib.h, figure out how that would work license wise
#ifndef RAYLIB_H
typedef struct Vector2 {
    float x;                // Vector x component
    float y;                // Vector y component
} Vector2;

typedef struct Vector3 {
    float x;                // Vector x component
    float y;                // Vector y component
    float z;                // Vector z component
} Vector3;

typedef struct Color {
    unsigned char r;        // Color red value
    unsigned char g;        // Color green value
    unsigned char b;        // Color blue value
    unsigned char a;        // Color alpha value
} Color;
#endif

typedef uint32_t EntityFlags; /// Entity flags (bitmask)

#define ENTITY_IS_3D      ((EntityFlags)1 << 0) /// set this flag for 3D entities, otherwise it's 2D by default
#define ENTITY_DRAWABLE   ((EntityFlags)1 << 1) /// set this flag for entities that should be drawn
#define ENTITY_MOVABLE    ((EntityFlags)1 << 2) /// set this flag for entities that can move
#define ENTITY_COLLIDABLE ((EntityFlags)1 << 3) /// set this flag for entities that can collide with others

#define ENTITY_2D_OBJECT ( ENTITY_DRAWABLE | ENTITY_MOVABLE | ENTITY_COLLIDABLE)
#define ENTITY_3D_OBJECT (ENTITY_IS_3D | ENTITY_DRAWABLE | ENTITY_MOVABLE | ENTITY_COLLIDABLE)
#define ENTITY_LOGIC     (EntityFlags)0


/// Entity position union (2D or 3D)
/// Vector3 is first in theses unions to enable initialization with
/// both EntityPosition pos = {{x, y, z}}; and EntityPosition pos = {{x, y}};
/// the compiler will complain with -Wmissing-field-initializers in the second case...
typedef union EntityPos {
   Vector3 pos3d;
   Vector2 pos2d;
} EntityPos;

/// Entity size union (2D or 3D)
/// Vector3 is first in theses unions to enable initialization with
/// both EntitySize s = {{x, y, z}}; and EntitySize s = {{x, y}};
/// the compiler will complain with -Wmissing-field-initializers in the second case...
typedef union EntitySize {
   Vector3 size3d;
   Vector2 size2d;
} EntitySize;

/// Entity speed union (2D or 3D)
/// Vector3 is first in theses unions to enable initialization with
/// both EntitySpeed s = {{x, y, z}}; and EntitySpeed speed = {{x, y}};
/// the compiler will complain with -Wmissing-field-initializers in the second case...
typedef union EntitySpeed {
   Vector3 speed3d;
   Vector2 speed2d;
} EntitySpeed;

/// the main entity struct
typedef struct entity {
   bool           is_valid;      /// set to false on `entity_create()` failure
   EntityFlags    flags;         /// entity flags (bitmask)
   EntityPos      pos;           /// position (union of Vector2 and Vector3)
   EntitySize     size;          /// size (union of Vector2 and Vector3)
   EntitySpeed    speed;         /// speed (union of Vector2 and Vector3)
   Color          color;
   char        name[256];
   char        uuid[UUID4_LEN];

   struct entity  *parent;

   size_t         children_count;
   size_t         children_capacity;
   struct entity  *children;

   void (*draw_func)(struct entity *entity);

} Entity;

void debug_print_entity_flags(EntityFlags flags);
void debug_print_entity_flags_table(EntityFlags flags);
void debug_entity_flags_string(EntityFlags flags, char *buffer, size_t buffer_size);

/// Resize the children array of an entity
/// TODO: remove this from the public API as this is managed internally by `entity_add_children()`
void entity_children_resize(Entity *entity, size_t new_capacity);

/// Add a child (passed by value). The memory of children is managed by the parent entity. You can free it with `entity_destroy_children(parent)`.
int entity_add_children(Entity *parent, Entity child);

// TODO: implement entity_remove_child(Entity *parent, size_t index);

/// Count direct children (not recursive) of an entity.
/// You can also use `entity_recursive_count_children()` for the recursive version.
int entity_count_children(Entity *parent);

/// Count all descendants (recursive) of an entity, up to a certain depth limit.
int entity_recursive_count_children(Entity *parent, int depth_limit);

/// return true if the entity has no parent
bool entity_is_root(Entity *entity);

/// Check if `possible_ancestor` is somewhere in the ancestry tree of `entity`
bool entity_is_ancestor_of(Entity *possible_ancestor, Entity *entity);

/// Check if `possible_descendant` is somewhere in the children tree of `entity`
bool entity_is_descendant_of(Entity *possible_descendant, Entity *entity);

/// Get a child entity by index in the children array
Entity *entity_get_child(Entity *entity, size_t index);

/// Get the parent entity (returns NULL on root entities)
Entity *entity_get_parent(Entity *entity);

// Get the base/root entity of the hierarchy (first entity without a parent in the tree)
Entity *entity_get_root(Entity *entity);

/// Create a new entity with specified parameters (Entity is returned by value, so NOT allocated on the heap, the memory is managed by the caller.)
/// If you use `entity_add_children()`, the memory will be managed by the parent entity, free it with `entity_destroy_children()`.
Entity entity_create(EntityFlags flags, const char* name, EntityPos pos, EntitySize size, Color color, size_t children_capacity);

// This should probably have a depth limit parameter (stack overflow risk)
// TODO = should be wrapped in a function that sets depth to 0 automatically

/// You must call this with depth = 0 initially
/// call a function against each children recursively (called on parent first in the depth order)
void entity_foreach_children_recursive(Entity *entity, int depth, void (*callback)(struct entity *entity, int depth));


/// TODO: figure out if this should be part of the public API or not, the functions names are getting pretty long and this could be done from within the callback
/// Same as `entity_foreach_children_recursive()` but accepts a filter function to only call the callback on entities that pass the filter
void entity_foreach_children_recursive_filtered( Entity *entity, int depth, void (*callback)(struct entity *entity, int depth), bool (*filter)(struct entity *entity));

/// Destroy recursively all children of an entity and free their memory
void entity_destroy_children(Entity *entity);


#endif // ENTITY_H
