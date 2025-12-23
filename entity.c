#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "entity.h"
#include <string.h>


void entity_childs_resize(Entity *self, size_t new_capacity) {
   assert(self != NULL /* attempting to resize NULL entity */);
   if (new_capacity < self->child_count) {
      self->child_count = new_capacity;
   }
   self->childs = (Entity *)realloc(self->childs, sizeof(Entity) * new_capacity);
   self->child_capacity = new_capacity;
}


bool entity_is_ancestor(Entity *self, Entity *possible_ancestor) {
   while (self != NULL) {
      if (self == possible_ancestor) return true;
      self = self->parent;
   }
   return false;
}


bool entity_is_descendant(Entity *self, Entity *possible_descendant) {
   return entity_is_ancestor(possible_descendant, self);
}




bool entity_childs_contains(Entity *self, Entity *possible_child) {
   for (size_t i = 0; i < self->child_count; i++) {
      if (&self->childs[i] == possible_child) {
         return true;
      }
   }
   return false;
}


int entity_childs_append(Entity *self, Entity child) {
   assert(self != NULL /* attempting to append to NULL entity */);

   // check for cycles
   if (entity_is_ancestor(self, &child)) {
      fprintf(stderr, "Error: attempting to append child that would create a cycle in entity tree\n"); //NOLINT
      return -1;
   }

   if (self->child_count >= self->child_capacity) {
      if (self->child_capacity == 0) {
         self->child_capacity = CHILDS_DEFAULT_CAPACITY;
      } else {
         self->child_capacity *= 2;
      }
      entity_childs_resize(self, self->child_capacity);
   }
   assert(self->child_count < self->child_capacity /* child count exceeds capacity after resize */);
   self->childs[self->child_count] = child;
   self->child_count++;
   return 0;
}


Entity *entity_childs_get(Entity *self, size_t index) {
   return (index < self->child_count) ? &self->childs[index] : NULL;
}


int entity_childs_count(Entity *self) {
   return self->child_count;
}


int entity_childs_recursive_count(Entity *self, int depth_limit) {
   if (depth_limit == 0) {
      fprintf(stderr, "Warning: depth limit reached in entity_childs_recursive_count\n"); //NOLINT
      return 0;
   }
   int total = self->child_count;
   for (size_t i = 0; i < self->child_count; i++) {
      total += entity_childs_recursive_count(&self->childs[i], depth_limit - 1);
   }
   return total;
}


Entity *entity_get_parent(Entity *self) {
   return self->parent;
}

bool entity_is_root(Entity *self) {
   return (self->parent == NULL);
}

Entity *entity_get_root(Entity *self) {
   Entity *current = self;
   while (!entity_is_root(current)) {
      current = current->parent;
   }
   return current;
}


Entity *entity_create(
      EntityType type,
      char *name,
      Vector2 position,
      Vector2 size,
      Color color,
      size_t child_capacity
      ) {

   Entity *new_entity = (Entity *)malloc(sizeof(Entity));
   assert(new_entity != NULL /* memory allocation failed for new entity */);

   new_entity->type     = type;
   new_entity->position = position;
   new_entity->size     = size;
   new_entity->color    = color;
   new_entity->speed    = (Vector2){0};

   new_entity->parent         = NULL;
   new_entity->child_count    = 0;
   new_entity->child_capacity = 0;
   new_entity->childs         = NULL;

   if (name != NULL) {
      assert(strlen(name) < sizeof(new_entity->name) /* entity name too long (limited at 256 chars*/);
      snprintf(new_entity->name, sizeof(new_entity->name), "%s", name); //NOLINT
   } else {
      new_entity->name[0] = '\0';
   }

   if (child_capacity > 0) {
      entity_childs_resize(new_entity, child_capacity);
   }

   new_entity->draw_self = NULL;

   return new_entity;
}


void free_entity(Entity *self) {
   assert(self != NULL /* attemptying to free NULL entity */);

   if (self->childs != NULL) {
      for (size_t i = 0; i < self->child_count; i++) {
         free_entity(&self->childs[i]);
      }
      free(self->childs);
      self->childs = NULL;
   }
}
