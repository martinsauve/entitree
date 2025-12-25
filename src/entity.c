#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "entity.h"
#include <string.h>
#include <errno.h>
#include <limits.h>


void entity_children_resize(Entity *entity, size_t new_capacity) {
   assert(entity != NULL /* attempting to resize NULL entity */);
   if (new_capacity < entity->children_count) {
      entity->children_count = new_capacity;
   }
   entity->children = (Entity *)realloc(entity->children, sizeof(Entity) * new_capacity);
   entity->children_capacity = new_capacity;
}


bool entity_is_ancestor_of(Entity *possible_ancestor, Entity *entity) {
   while (entity != NULL) {
      if (entity == possible_ancestor) return true;
      entity = entity->parent;
   }
   return false;
}


bool entity_is_descendant_of(Entity *possible_descendant, Entity *entity) {
   return entity_is_ancestor_of(entity, possible_descendant);
}




bool entity_children_contains(Entity *self, Entity *possible_child) {
   for (size_t i = 0; i < self->children_count; i++) {
      if (&self->children[i] == possible_child) {
         return true;
      }
   }
   return false;
}


int entity_add_children(Entity *parent, Entity child) {
   assert(parent != NULL /* attempting to append to NULL entity */);

   // Does this check make sense anymore? We're now passing child by value
   // check for cycles
   if (entity_is_ancestor_of(parent, &child)) {
      fprintf(stderr, "Error: attempting to append child that would create a cycle in entity tree\n"); //NOLINT
      return EXIT_FAILURE;
   }

   if (parent->children_count >= parent->children_capacity) {
      if (parent->children_capacity == 0) {
         parent->children_capacity = CHILDREN_DEFAULT_CAPACITY;
      } else {
         parent->children_capacity *= 2;
      }
      entity_children_resize(parent, parent->children_capacity);
   }
   assert(parent->children_count < parent->children_capacity /* child count exceeds capacity after resize */);
   parent->children[parent->children_count] = child;
   parent->children_count++;
   return EXIT_SUCCESS;
}


Entity *entity_get_child(Entity *entity, size_t index) {
   return (index < entity->children_count) ? &entity->children[index] : NULL;
}


int entity_count_children(Entity *parent) {
   return parent->children_count;
}


int entity_recursive_count_children(Entity *parent, int depth_limit) {
   if (depth_limit == 0) {
      fprintf(stderr, "Warning: depth limit reached in entity_recursive_count_children\n"); //NOLINT
      errno = EOVERFLOW;
      return -1;
   }
   int total = parent->children_count;
   for (size_t i = 0; i < parent->children_count; i++) {
      if (total >= INT_MAX - 1) {
         fprintf(stderr, "Warning: integer overflow in entity_recursive_count_children\n"); //NOLINT
         errno = EOVERFLOW;
         return -1; // propagate depth limit reached
      }
      total += entity_recursive_count_children(&parent->children[i], depth_limit - 1);
   }
   return total;
}


Entity *entity_get_parent(Entity *entity) {
   return entity->parent;
}

bool entity_is_root(Entity *entity) {
   return (entity->parent == NULL);
}

Entity *entity_get_root(Entity *entity) {
   Entity *current = entity;
   while (!entity_is_root(current)) {
      current = current->parent;
   }
   return current;
}

Entity entity_create(
      EntityType type,
      const char *name,
      Vector2 position,
      Vector2 size,
      Color color,
      size_t children_capacity
      )
{

   Entity new_entity = {0};

   new_entity.type     = type;
   new_entity.position = position;
   new_entity.size     = size;
   new_entity.color    = color;
   new_entity.speed    = (Vector2){0};

   // generate UUID4
   if (uuid4_init() == UUID4_EFAILURE) {
      errno = EIO;
      return (Entity){0};
   }
   uuid4_generate(new_entity.uuid);

   if (name != NULL) {
      if (strlen(name) >= sizeof(new_entity.name)) {
         errno = EINVAL;
         return (Entity){0}; // name too long
      }
      snprintf(new_entity.name, sizeof(new_entity.name), "%s", name); //NOLINT
   } else new_entity.name[0] = '\0';


   // Pre-allocate children array
   if (children_capacity > 0) {
      new_entity.children = calloc(sizeof(Entity), children_capacity);
      if (new_entity.children == NULL){
         errno = ENOMEM;
         return (Entity){0}; // error allocating the children array
      }
      new_entity.children_capacity = children_capacity;
   }

   new_entity.is_valid = true;
   return new_entity;
}

// TODO: this function is recursive, figure out the implications for deep entity trees.

void entity_foreach_children_recursive(Entity *entity, int depth, void (*callback)(struct entity *entity, int depth)) {
   if (entity == NULL) {
      fprintf(stderr, "Warning: NULL entity passed to entity_foreach_children_recursive\n"); //NOLINT
      return;
   }
   if (callback == NULL) {
      fprintf(stderr, "Warning: NULL callback passed to entity_foreach_children_recursive\n"); //NOLINT
      return;
   }
   // TODO: should we callback before or after recursing into children?
   callback(entity, depth);

   for (int i = 0; i < entity->children_count; ++i) {
      entity_foreach_children_recursive(&entity->children[i], depth + 1, callback);
   }
}

// TODO: this function is recursive, figure out the implications for deep entity trees.

void entity_destroy_children(Entity *entity) {
   assert(entity != NULL /* attemptying to free NULL entity */);

   if (entity->children != NULL) {
      for (size_t i = 0; i < entity->children_count; i++) {
         entity_destroy_children(&entity->children[i]);
      }
      free(entity->children);
      entity->children = NULL;
      entity->children_count = 0;
      entity->children_capacity = 0;
   }
}
