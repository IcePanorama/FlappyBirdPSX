#ifndef _PIPE_MANAGER_H_
#define _PIPE_MANAGER_H_

#include "game_obj/pipes.h"
#include "sys/fb_ints.h"

void pm_init_pipe_manager (void);
void pm_manage_pipes (void);
void pm_spawn_pipe_entity (void);
PipesEntity_t *pm_get_pipe (uint8_t u8_eid);

#endif /* _PIPE_MANAGER_H_ */
