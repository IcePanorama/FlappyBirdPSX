#ifndef _COMMAND_LIST_H_
#define _COMMAND_LIST_H_

#include "game/gmstate.h"
#include "input/command.h"

#define CMDLIST_MAX_NUM_CMDS (2)

typedef struct CommandList_s
{
  Command_t *list[(CMDLIST_MAX_NUM_CMDS)];
  uint8_t u8_num_elements;
} CommandList_t;

extern CommandList_t *cmdl_command_lists[GSTATE_NUM_STATES];

#endif /* _COMMAND_LIST_H_ */
