#ifndef _PIPES_H_
#define _PIPES_H_

#include "colshape.h"
#include "sprites.h"
#include "stdbool.h"
#include "vec2.h"

typedef struct Pipes_s
{
  /*
   *  fixme: change to `in_use`--more correct term probably. `active` suggests
   *  it may be temporarily (at least, in my mind, it does).
   */
  bool_t active;
  Vec2_t position;
  Sprite_t *top_sprite_component;
  Sprite_t *bot_sprite_component;
  ColShape_t *top_col_shape;
  ColShape_t *bot_col_shape;
} Pipes_t;

Pipes_t create_pipes_entity (void);
void update_pipes_entity (Pipes_t *p);

#endif /* _PIPES_H_ */
