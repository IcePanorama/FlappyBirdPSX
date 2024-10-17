#ifndef _PHYSICS_COMPONENT_H_
#define _PHYSICS_COMPONENT_H_

#include "sys/fb_ints.h"
#include "sys/fb_vecs.h"

#define PHYSICS_MAX_NUM_PHYSICS_COMP (32)

typedef struct PhysicsCompnt_s
{
  int8_t i8_parent_id;
  Vec2_t v2_position;
  int32_t i32_velocity;
} PhysicsCompnt_t;

extern PhysicsCompnt_t pc_physics_pool[(PHYSICS_MAX_NUM_PHYSICS_COMP)];
extern uint8_t u8_pc_num_physics;

void create_new_physics_compnt (int8_t i8_id);
void destroy_physics_compnt (int8_t i8_id);
PhysicsCompnt_t *get_physics_compnt_with_id (int8_t i8_id);
void update_physics_compnt (PhysicsCompnt_t *pc, Vec2_t *v2_output_pos);

#endif /* _PHYSICS_COMPONENT_H_ */
