#include "input/controlr.h"

#include <libpad.h>
#include <stdio.h>
#include <sys/types.h>

// names are 1-indexed as that's how it's written on the PSX itself
u_char uc_controller_1[34];
u_char uc_controller_2[34];

static Command_t *get_command (CommandList_t *cmdl, int32_t i32_padd);

void
ctrl_init_controllers (void)
{
  PadInitDirect (uc_controller_1, uc_controller_2);
  PadStartCom ();
}

void
ctrl_handle_user_input (CommandList_t *cmdl, void *entity)
{
  Command_t *cmd;
  int32_t i32_padd;

  if (cmdl == 0) return;

//TODO: expand to support two players.
  if (uc_controller_1[0] != 0)
  {
#ifdef DEBUG_BUILD
    printf ("Padd error!\n");
#endif /* DEBUG_BUILD */
    return;
  }

  i32_padd = ~((uc_controller_1[2] << 8) | (uc_controller_1[3]));
  cmd = get_command (cmdl, i32_padd);
  if (cmd)
    cmd->execute(entity);
}

Command_t *
get_command (CommandList_t *cmdl, int32_t i32_padd)
{
  uint8_t i;
  uint16_t u16_mapped_button;
  static int32_t i32_last_padd = 0;

  for (i = 0; i < cmdl->u8_num_elements; i++)
  {
    u16_mapped_button = cmdl->list[i]->u16_mapped_button;
    if ((i32_padd & u16_mapped_button) && !(i32_last_padd & u16_mapped_button))
    {
      i32_last_padd = i32_padd;
      return cmdl->list[i];
    }
  }

  i32_last_padd = i32_padd;
  return 0;
}
