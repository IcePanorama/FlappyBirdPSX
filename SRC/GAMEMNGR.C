#include "gamemngr.h"
#include "command.h"
#include "fb_defs.h"
#include "gmstate.h"
#include "pipes.h"
#include "stdbool.h"
#include "stdint.h"
#include "time.h"

// sys/types.h MUST be included before libetc otherwise CC complains.
#include <sys/types.h>
#include <libetc.h>
#include <rand.h>

#ifdef DEBUG_BUILD
  #include <assert.h>
  #include <stdio.h>
#endif /* DEBUG_BUILD */

#define NUM_COMMANDS (1)
#define MAX_NUM_PIPES (10)

void init_rng_system ();

/*
 *  TODO: load commands and command sets from disk, optionally save user-defined
 *  input maps to memory cards.
 */
Command_t jump_command = { PADRdown, player_jump };
/** Any button press should init the rng system. */
Command_t seed_rng_command = { ((uint16_t)-1), init_rng_system };

Command_t *gstate_normal_command_set[NUM_COMMANDS] = { &jump_command };
Command_t *gstate_game_start_command_set[NUM_COMMANDS] = { &seed_rng_command };

static void manage_pipes (void);
static void spawn_new_pipe (void);
static void update_game_state_normal (void);
static void update_game_state_game_start (void);

// TODO: move the pipes pool/related stuff to `pipes.h`.
static Pipes_t pipes_pool[MAX_NUM_PIPES] = {{0}};
/**
 *  The most recently created pipe. Probably can stay here as this file will
 *  still be managing when pipes spawn(/despawn ?).
 */
static Pipes_t *last_pipe = 0;
static GameState_t curr_game_state = GSTATE_GAME_START;
static bool_t is_rng_seeded = FALSE;

void
init_game_state (void)
{
  player = create_player_entity (SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
  load_command_list (gstate_game_start_command_set, NUM_COMMANDS);
}

void
update_game_state (void)
{
  switch (curr_game_state)
  {
    case GSTATE_NORMAL:
      update_game_state_normal ();
      break;
    case GSTATE_GAME_START:
      update_game_state_game_start ();
      break;
    case GSTATE_GAME_PAUSED:
    case GSTATE_GAME_OVER:
    default:
      break;
  }
}

void
update_game_state_normal (void)
{
  if (!player.is_alive)
  {
    curr_game_state = GSTATE_GAME_OVER;
    return;
  }

  update_player_entity (&player);
  manage_pipes ();
}

void
update_game_state_game_start (void)
{
  if (is_rng_seeded)
  {
    curr_game_state = GSTATE_NORMAL;
    load_command_list (gstate_normal_command_set, NUM_COMMANDS);
  }
}

void
manage_pipes (void)
{
  uint8_t i;

  // TODO: decrease space between pipes over time.
  if ((last_pipe == 0) || (SCREEN_WIDTH - last_pipe->position.x >= 128))
    spawn_new_pipe ();

  for (i = 0; i < MAX_NUM_PIPES; i++)
  {
    if (pipes_pool[i].active)
      update_pipes_entity(&pipes_pool[i]);
  }
}

void
spawn_new_pipe (void)
{
  uint8_t i;
  for (i = 0; i < MAX_NUM_PIPES; i++)
  {
    if (pipes_pool[i].active == FALSE)
    {
      pipes_pool[i] = create_pipes_entity ();
      last_pipe = &pipes_pool[i];
      return;
    }
  }

#ifdef DEBUG_BUILD
  printf ("ERROR: Pipes pool is full.\n");
  assert(FALSE);
#endif /* DEBUG_BUILD */
}

void
init_rng_system ()
{
  uint8_t seed;

  seed = TIME_GET_CURRENT_TIME();
#ifdef DEBUG_BUILD
  printf ("Seed: %d\n", seed);
#endif /* DEBUG_BUILD */
  srand (seed);
  is_rng_seeded = TRUE;
}

#undef NUM_COMMANDS
#undef MAX_NUM_PIPES
