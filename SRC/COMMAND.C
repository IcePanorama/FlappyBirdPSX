#include "command.h"

#define CMD_MAX_NUM_COMMANDS (1)

// sys/types.h MUST be included before libetc otherwise CC complains.
#include <sys/types.h>
#include <libetc.h>
#include <stdio.h>

/*
void execute_cmd_button_x (void);

/ List of commands /
Command_t cmd_button_x = { PADRdown, execute_cmd_button_x };
*/
Command_t *command_list[CMD_MAX_NUM_COMMANDS] = { 0 };

void
change_command_button (Command_t *c, uint16_t button)
{
  c->button = button;
}

Command_t *
get_command (int32_t padd, int32_t last)
{
  uint32_t i;

  for (i = 0; i < CMD_MAX_NUM_COMMANDS; i++)
  {
    if ((padd & command_list[i]->button) && !(last & command_list[i]->button))
      return command_list[i];
  }

  return 0;
}

/*
void
execute_cmd_button_x (void)
{
  printf ("X button pressed!\n");
}
*/

void
load_command_list (Command_t **list, uint32_t len)
{
  uint32_t i;
  for (i = 0; i < len; i++)
  {
    command_list[i] = list[i];
  }

  for (; i < CMD_MAX_NUM_COMMANDS; i++)
  {
    command_list[i] = 0;
  }
}

#undef CMD_MAX_NUM_COMMANDS
