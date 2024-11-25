#ifndef _COLLISION_MANAGER_H_
#define _COLLISION_MANAGER_H_

#include "game_obj/player.h"

void cm_init_collision_manager (PlayerEntity_t *p);
void cm_handle_collisions (void);

#endif /* _COLLISION_MANAGER_H_ */
