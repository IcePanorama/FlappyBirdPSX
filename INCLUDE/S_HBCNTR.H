#ifndef _S_HBLANK_COUNTER_H_
#define _S_HBLANK_COUNTER_H_

#include <kernel.h>
#include <libapi.h>

void s_init_hblanks_counter(void);
/**
 *  Time between two frames. In order for this to work properly, you MUST call
 *  `s_sys_start` before any other functions in your main game loop and you MUST
 *  call `s_sys_end ()` as the last function in your main game loop.
 */
int s_delta_time(void);
void s_sys_start(void);
void s_sys_end(void);

#endif /* _S_HBLANK_COUNTER_H_ */
