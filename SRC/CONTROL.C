#include "control.h"
#include "command.h"
#include "stdint.h"

#include <libpad.h>
#include <sys/types.h>

#ifdef DEBUG_BUILD
  #include <stdio.h>
#endif /* DEBUG_BUILD */

static u_char uc_controller_1[34];
static u_char uc_controller_2[34];

void
init_controllers (void)
{
  PadInitDirect (uc_controller_1, uc_controller_2);
  PadStartCom ();
}

void
handle_user_input (void *entity)
{
  static int32_t i32_last_padd = 0;
  int32_t i32_padd;
  Command_t *command;

  if (uc_controller_1[0] != 0)
  {
#ifdef DEBUG_BUILD
    printf ("Padd error!\n");
#endif /* DEBUG_BUILD */
    return;
  }

  i32_padd = ~((uc_controller_1[2] << 8) | (uc_controller_1[3]));

  command = get_command (i32_padd, i32_last_padd);
  if (command)
    command->execute (entity);

  i32_last_padd = i32_padd;
}
