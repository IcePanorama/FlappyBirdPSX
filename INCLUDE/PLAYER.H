#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "colshape.h"
#include "sprites.h"
#include "stdbool.h"
#include "vec2.h"

typedef struct Player_s
{
  bool_t is_alive;
  Vec2_t position;
  uint16_t width;
  uint16_t height;
  int32_t velocity_y;
  Sprite_t *sprite_component;
  ColShape_t *col_shape;
} Player_t;

Player_t create_player_entity (uint16_t start_x, uint16_t start_y);
void update_player_entity (Player_t *p);
void player_jump (void *p);

#endif
