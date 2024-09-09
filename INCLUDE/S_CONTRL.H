#ifndef _S_CONTROLLER_H_
#define _S_CONTROLLER_H_

#include <libpad.h>
#include <sys/types.h>

// Should these be merged together?
// Indexing from 1 as that's what the PSX says on the front of the unit
u_char controller_1[34];
u_char controller_2[34];

void s_init_controllers (void);

#endif /* _S_CONTROLLER_H_ */
