#include "entity.h"
#include <raylib.h>
#include <raymath.h>

typedef struct {
   float angle;
   Model model;
} UserData;

// TODO: entity3d are really voxels for now, introducing the all models stuff is another can of worms, the current system is not really designed for that.
void draw_entity_3d(Entity *entity) {
   float angle = ((UserData *)(entity->user_data))->angle;
   Model model = ((UserData *)(entity->user_data))->model;
   DrawModelEx(
         model,
         entity->pos.as_vec3,
         (Vector3){1,1,1},
         angle,
         entity->size.as_vec3,
         entity->color
         );

   //DrawCubeWiresV(entity->pos.as_vec3, entity->size.as_vec3, entity->color);
}

// TODO: should be provided by entiTree as a helper function
void destroy_cube_data(Entity *entity) {
   printf("[EntiTree]: Freeing cube data for entity: %s\n", entity->name);
   UnloadModel(((UserData *)(entity->user_data))->model);
   free(entity->user_data);
}

void draw_entity_2d(Entity *entity) {

   DrawRectangleV(
         entity_relative_position(entity).as_vec2,
         entity->size.as_vec2,
         entity->color
         );
}

// TODO: should be provided by entiTree as a helper function
void draw_callback(struct entity *entity, int depth) {
   (void)depth; // unused
   DRAW(entity);
}

bool entity_is_3d_object(struct entity *entity) {
   return entity->flags & ENTITY_3D_OBJECT;
}

bool entity_is_2d_object(struct entity *entity) {
   return entity->flags & ENTITY_2D_OBJECT;
}

// No filter
bool entity_all(struct entity *entity) {
   (void)entity; // unused
   return true;
}

int main() {
   // Initialize Raylib Window
   const int screenWidth = 800;
   const int screenHeight = 600;
   InitWindow(screenWidth, screenHeight, "EntiTree with Raylib (2D and 3D example)");
   SetTargetFPS(60); // Set frame rate

   // Root entity (manages children)
   Entity root = entity_create(ENTITY_LOGIC, "Root Entity", (EntityPos){{0, 0}}, (EntitySize){{0, 0}}, (Color){0, 0, 0, 0}, 0);

   // Create a 2D entity
   Entity *box = entity_add_children(&root, entity_create(
            ENTITY_2D_OBJECT,
            "2D Box",
            (EntityPos){{100, 100}},                // Position in 2D space
            (EntitySize){{50, 50}},                // Size (width, height)
            RED,                                   // Color
            0                                      // No children for this entity
            ));
   box->draw_func = draw_entity_2d;

  Entity *relative = entity_add_children(box, entity_create(
            ENTITY_2D_OBJECT,
            "Relative Box",
            (EntityPos){{25, 25}},                // Position in 2D space
            (EntitySize){{20, 20}},                // Size (width, height)
            DARKGREEN,                                   // Color
            0                                      // No children for this entity
            ));
   relative->draw_func = draw_entity_2d;


   // Create a 3D cube entity
   Entity *cube = entity_add_children(&root, entity_create(
            ENTITY_3D_OBJECT,
            "3D Cube",
            (EntityPos){{1.0f, 1.5f, -1.0f}},             // Position in 3D space
            (EntitySize){{1.0f, 1.0f, 1.0f}},      // Size (width, height, depth)
            GREEN,                                 // Color
            0                                      // No children for this entity
            ));
   cube->draw_func = draw_entity_3d;

   cube->user_data = malloc(sizeof(UserData));
   cube->destroy_user_data = destroy_cube_data; // defer-like cleanup, called on destroy_children, would really benefit from having anonymous functions / lambdas, but C...

   ((UserData *)(cube->user_data))->model =
      LoadModelFromMesh(
         GenMeshCube(
            cube->size.as_vec3.x,
            cube->size.as_vec3.y,
            cube->size.as_vec3.z
            )
         );

   ((UserData *)(cube->user_data))->angle = 0.0f;
   // TODO: this is not a very elegant way to handle user data, might have to provide some helper functions. This cast syntax is ugly.
   // this is useful tho, but we might also want to provide a simpler key-value store for primitive data types.
   // figure out how this would work with FFI bindings.

   Entity *cube2 = entity_add_children(&root, entity_create(
            ENTITY_3D_OBJECT,
            "3D Cube2",
            (EntityPos){{-1, 1.5f, -1}},             // Position in 3D space
            (EntitySize){{1.0f, 1.0f, 1.0f}},      // Size (width, height, depth)
            BLUE,                                 // Color
            0                                      // No children for this entity
            ));
   cube2->draw_func = draw_entity_3d;
   cube2->user_data = malloc(sizeof(UserData));
   cube2->destroy_user_data = destroy_cube_data; // defer-like cleanup, called on destroy_children, would really benefit from having anonymous functions / lambdas, but C...

   ((UserData *)(cube2->user_data))->angle = 0.0f;
   ((UserData *)(cube2->user_data))->model =
      LoadModelFromMesh(
         GenMeshCube(
            cube2->size.as_vec3.x,
            cube2->size.as_vec3.y,
            cube2->size.as_vec3.z
            )
         );


   // 3D Camera setup
   Camera camera = { 0 };
   camera.position = (Vector3){ 5.0f, 5.0f, 5.0f };   // Camera position in 3D space
   camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };     // Camera looks at the origin
   camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };         // Camera up direction
   camera.fovy = 45.0f;                               // Camera field-of-view

   while (!WindowShouldClose()) {
      box->pos.as_vec2.x += 2;
      ((UserData *)(cube->user_data))->angle += 1.0f;
      if (box->pos.as_vec2.x > screenWidth) box->pos.as_vec2.x = 0;
      //RMAPI Vector3 Vector3RotateByAxisAngle(Vector3 v, Vector3 axis, float angle)

      BeginDrawing();

         ClearBackground(RAYWHITE);

         BeginMode2D((Camera2D){{0}, {0}, 0, 1.0f});
            DrawFPS(10, 10);
            entity_foreach_children_recursive_filtered(&root, 0, draw_callback, entity_is_2d_object);
         EndMode2D();

         BeginMode3D(camera);
            entity_foreach_children_recursive_filtered(&root, 0, draw_callback, entity_is_3d_object); // Draw all 3D entities
         EndMode3D();

      EndDrawing(); // End drawing
   }

   // Clean up
   CloseWindow(); // Close Raylib
   entity_destroy_children(&root);

   return 0;
}
