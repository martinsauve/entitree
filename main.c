#include <raylib.h>
#include <raymath.h>
#include <time.h>
#include <assert.h>
#include "entity.h"

#define SHOW_FPS
#define RL_WINDOW_CONFIG_FLAGS (FLAG_MSAA_4X_HINT | FLAG_WINDOW_UNDECORATED )
#define SCREEN_FPS 120.0
#define SCREEN_WIDTH  512
#define SCREEN_HEIGHT 512

#define LEFTVEC   (Vector2){-1, 0}
#define RIGHTVEC  (Vector2){ 1, 0}
#define UPVEC     (Vector2){ 0,-1}
#define DOWNVEC   (Vector2){ 0, 1}

#define KEYBIND_LEFT       (key == KEY_LEFT  || key == KEY_H  || key == KEY_A)
#define KEYBIND_DOWN       (key == KEY_DOWN  || key == KEY_J  || key == KEY_S)
#define KEYBIND_UP         (key == KEY_UP    || key == KEY_K  || key == KEY_W)
#define KEYBIND_RIGHT      (key == KEY_RIGHT || key == KEY_L  || key == KEY_D)

#define BULLET_MAX 16

typedef enum direction {
   LEFT,
   RIGHT,
   UP,
   DOWN
} Direction;


void draw_player(Entity *entity)
{
   assert(entity != NULL && entity->type == ENTITY_PLAYER);

   DrawRectangleV(entity->position, entity->size, entity->color);
}

void drawEntity(Entity *entity)
{
   assert(entity != NULL && entity->type != ENTITY_NONE);

   DrawRectangleV(entity->position, entity->size, entity->color);
}

void handleInput(Entity *entity)
{
   const float acceleration = 0.5f;

   if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_H) || IsKeyDown(KEY_A))
   {
      entity->speed.x -= acceleration;
   }
   if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_L) || IsKeyDown(KEY_D))
   {
      entity->speed.x += acceleration;
   }
   if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_K) || IsKeyDown(KEY_W))
   {
      entity->speed.y -= acceleration;
   }
   if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_J) || IsKeyDown(KEY_S))
   {
      entity->speed.y += acceleration;
   }
}

void applyFriction(Entity *entity, Vector2 friction)
{
   entity->speed.x *= friction.x;
   entity->speed.y *= friction.y;
}

void updatePosition(Entity *entity)
{
   entity->position.x += entity->speed.x;
   entity->position.y += entity->speed.y;

   // Keep entity within screen bounds
   if (entity->position.x < 0) entity->position.x = 0;
   if (entity->position.y < 0) entity->position.y = 0;
   if (entity->position.x + entity->size.x > SCREEN_WIDTH)
      entity->position.x = SCREEN_WIDTH - entity->size.x;
   if (entity->position.y + entity->size.y > SCREEN_HEIGHT)
      entity->position.y = SCREEN_HEIGHT - entity->size.y;
}

int main(void)
{
   Entity player = {0};
   player.type     = ENTITY_PLAYER;
   player.position = (Vector2){ SCREEN_WIDTH/2.0f - 16, SCREEN_HEIGHT - 48 };
   player.size     = (Vector2){ 32, 32 };
   player.color    = BLUE;
   player.speed    = (Vector2){0};

   const Vector2 friction = (Vector2){ 0.9f, 0.9f };


   // entity test
   // RL SETUP
   SetRandomSeed(time(NULL));
   SetConfigFlags(RL_WINDOW_CONFIG_FLAGS);
   InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "invaders");
   SetTargetFPS(SCREEN_FPS);


   while (!WindowShouldClose())
   {
      BeginDrawing();
      {
         ClearBackground(WHITE);
#ifdef SHOW_FPS
         DrawFPS(10, 10);
#endif
         handleInput(&player);
         applyFriction(&player, friction);
         updatePosition(&player);
         drawEntity(&player);
      }
      EndDrawing();
   }
   return 0;
}
