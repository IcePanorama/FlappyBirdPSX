//FIXME: move to appropriate subdir
/**
 *  COMMAND - a simple implementation of the 'command' design pattern.
 *  @see: en.wikipedia.org/wiki/Command_pattern
 */
#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "sys/fb_ints.h"

typedef struct Command_s
{
  uint16_t u16_mapped_button; // see `libetc.h` for list of possible values
  void (*const execute)();
} Command_t;

#endif /* _COMMAND_H_ */
