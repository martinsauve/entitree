#ifndef ENTITY_H
#define ENTITY_H
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../third_party/uuid4/src/uuid4.h"
#include "../third_party/cJSON/cJSON.h"
#include <assert.h>
#include <stdio.h>

#define CHILDREN_DEFAULT_CAPACITY 64

/// The types Vector2, Vector3 and Color are compatible with (and provided by) raylib. You can use <raymath.h> functions to manipulate them.

/*************************************************************************************************************/
// TODO: thoses are copy pasted from raylib.h, figure out how that would work license wise
#if !defined(RL_VECTOR2_TYPE)
// Vector2 type
typedef struct Vector2 {
    float x;
    float y;
} Vector2;
#define RL_VECTOR2_TYPE
#endif

#if !defined(RL_VECTOR3_TYPE)
// Vector3 type
typedef struct Vector3 {
    float x;
    float y;
    float z;
} Vector3;
#define RL_VECTOR3_TYPE
#endif

#if !defined(RL_COLOR_TYPE)
typedef struct Color {
    unsigned char r;        // Color red value
    unsigned char g;        // Color green value
    unsigned char b;        // Color blue value
    unsigned char a;        // Color alpha value
} Color;
#define RL_COLOR_TYPE
#endif


/*************************************************************************************************************/



typedef uint32_t EntityFlags; /// Entity flags (bitmask)

#define ENTITY_IS_3D      ((EntityFlags)1 << 0) /// set this flag for 3D entities, otherwise it's 2D by default
#define ENTITY_DRAWABLE   ((EntityFlags)1 << 1) /// set this flag for entities that should be drawn
#define ENTITY_MOVABLE    ((EntityFlags)1 << 2) /// set this flag for entities that can move
#define ENTITY_COLLIDABLE ((EntityFlags)1 << 3) /// set this flag for entities that can collide with others

#define ENTITY_2D_OBJECT ( ENTITY_DRAWABLE | ENTITY_MOVABLE | ENTITY_COLLIDABLE)                /// 2D object entity convenience flag
#define ENTITY_3D_OBJECT (ENTITY_IS_3D | ENTITY_DRAWABLE | ENTITY_MOVABLE | ENTITY_COLLIDABLE)  /// 3D object entity convenience flag
#define ENTITY_LOGIC     (EntityFlags)0                                                         /// logic-only entity convenience flag (no drawable, no movable, no collidable)


/// Entity position union (2D or 3D)
/// Vector3 is first in theses unions to enable initialization with
/// both EntityPosition pos = {{x, y, z}}; and EntityPosition pos = {{x, y}};
/// the compiler will complain with -Wmissing-field-initializers in the second case...
typedef union EntityPos {
   Vector3 as_vec3;
   Vector2 as_vec2;
} EntityPos;

/// Entity size union (2D or 3D)
/// Vector3 is first in theses unions to enable initialization with
/// both EntitySize s = {{x, y, z}}; and EntitySize s = {{x, y}};
/// the compiler will complain with -Wmissing-field-initializers in the second case...
typedef union EntitySize {
   Vector3 as_vec3;
   Vector2 as_vec2;
} EntitySize;

/// Entity speed union (2D or 3D)
/// Vector3 is first in theses unions to enable initialization with
/// both EntitySpeed s = {{x, y, z}}; and EntitySpeed speed = {{x, y}};
/// the compiler will complain with -Wmissing-field-initializers in the second case...
typedef union EntitySpeed {
   Vector3 as_vec3;
   Vector2 as_vec2;
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

   void           *user_data;                               /// pointer to user data (can be NULL)
   void           (*destroy_user_data)(struct entity *entity);    /// function to destroy data (can be NULL), called on `entity_destroy_children()`, optional, you could also manage memory manually
   cJSON          (*user_data_to_json)(void *user_data);      /// function to serialize user data to JSON (can be NULL), optional
   void           *(*user_data_from_json)(cJSON *json);       /// function to deserialize user data from JSON (can be NULL), optional

   struct entity  *parent;

   size_t         children_count;
   size_t         children_capacity;
   struct entity  *children;

   void (*draw_func)(struct entity *entity);

} Entity;

#define DRAW(entity_ptr) do {                                                             \
   assert((entity_ptr) != NULL /*entity is NULL*/);                                       \
   if ((entity_ptr)->draw_func != NULL) {                                                 \
         (entity_ptr)->draw_func(entity_ptr);                                             \
   } else {                                                                               \
      fprintf(stderr, "[EntiTree]: Warning: entity '%s' has no draw_func set\n", (entity_ptr)->name);/*NOLINT*/ \
   }                                                                                      \
} while(0)

/// A callback function that calls the entity's draw function. Useful for `entity_foreach_children_recursive()`. It simply calls the DRAW() macro but accepts
/// the required signature for the callback. This is a convenience function, please do not hesitate to write your own custom draw callback if needed.
void draw_callback(struct entity *entity, int depth);

void debug_print_entity_flags(EntityFlags flags);
void debug_print_entity_flags_table(EntityFlags flags);
void debug_entity_flags_string(EntityFlags flags, char *buffer, size_t buffer_size);


/// Add a child (passed by value). The memory of children is managed by the parent entity. You can free it with `entity_destroy_children(parent)`.
/// Returns a pointer to the newly added child within the parent's children array.
Entity *entity_add_children(Entity *parent, Entity child);

// TODO: implement entity_remove_child(Entity *parent, size_t index);

/// Count direct children (not recursive) of an entity.
/// You can also use `entity_recursive_count_children()` for the recursive version.
int entity_count_children(Entity *parent);

/// Count all descendants (recursive) of an entity, up to a certain depth limit.
int entity_recursive_count_children(Entity *parent, int depth_limit);

/// return true if the entity has no parent
bool entity_is_root(Entity *entity);

/// A filter function that returns true if the the flags ENTITY_3D_OBJECT are set. Useful for `entity_foreach_children_recursive_filtered()`.
bool entity_is_3d_object(Entity *entity);

/// A filter function that returns true if the the flags ENTITY_2D_OBJECT are set. Useful for `entity_foreach_children_recursive_filtered()`.
bool entity_is_2d_object(Entity *entity);

/// Check if `possible_ancestor` is somewhere in the ancestry tree of `entity`
bool entity_is_ancestor_of(Entity *possible_ancestor, Entity *entity);

/// Check if `possible_descendant` is somewhere in the children tree of `entity`
bool entity_is_descendant_of(Entity *possible_descendant, Entity *entity);

/// Get the computed position of an entity relative to the world (taking into account parent positions)
EntityPos entity_relative_position(Entity *entity);

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

/// Default function to destroy user data (simply frees the pointer)
void default_destroy_user_data(Entity *entity);


#endif // ENTITY_H
