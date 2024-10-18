#ifndef _CONTROLLERS_H_
#define _CONTROLLERS_H_

#include "input/cmdlist.h"

void ctrl_init_controllers (void);
void ctrl_handle_user_input (CommandList_t *cmdl, void *entity);

#endif /* _CONTROLLERS_H_ */
