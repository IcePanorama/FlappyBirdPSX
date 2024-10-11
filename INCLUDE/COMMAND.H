/**
 *  COMMAND.H - a simple implementation of the 'command' design pattern for user
 *  input.
 *
 *  @see: en.wikipedia.org/wiki/Command_pattern
 */
#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "stdint.h"

typedef struct Command_s
{
  uint16_t button;             // see `libetc.h` for a list of possible buttons
  /**
   *  Empty parameter list allows functions to optionally take a `void *` as
   *  input or just ignore it. `handle_user_input` defaults to providing one.
   */
  void (*const execute)();
} Command_t;

/** Returns the `Command_t` associated w/ some user input or NULL. */
Command_t *get_command (int32_t padd, int32_t last);
/** Change which button the given command is associated with. */
void change_command_button (Command_t *c, uint16_t button);
void load_command_list (Command_t **list, uint32_t len);

#endif /* _COMMAND_H_ */
