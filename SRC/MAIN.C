/** MAIN - Entry point of FlappyBirdPSX. */
#include "control.h"  // for `handle_user_input`
#include "gamemngr.h"
#include "stdint.h"
#include "time.h"
#include "video.h"

#ifdef DEBUG_BUILD
  #include <stdio.h>
#endif /* DEBUG_BUILD */

/*
 *  TODO: Adjust this value as needed.
 *  Smaller = Game appears smoother, more resource intensive.
 *  larger = Game appears choppier, less resource intensive.
 */
#define TICKS_PER_UPDATE (7)
/** Defining this as a macro simply to improve readability below. */
#define CALC_DELTA_TIME(curr, prev) \
  (((curr) >= (prev)) ? ((curr) - (prev)) : ((0xFF - (prev)) + (curr) + 1))

u_long _ramsize   = 0x00200000;  // Force 2 MB of RAM
u_long _stacksize = 0x00004000;  // Force 16 KB of stack

int
main (void)
{
  int16_t i16_prev_time;
  int16_t i16_curr_time;
  int16_t i16_dt;        // delta time
  int32_t i32_lag = 0;

  init_video_subsystem ();
  init_controllers (); // gamemngr could encapsulate this as well
  init_game_state ();

  i16_prev_time = TIME_GET_CURRENT_TIME();
  while (1)
  {
    i16_curr_time = TIME_GET_CURRENT_TIME();
    i16_dt = CALC_DELTA_TIME(i16_curr_time, i16_prev_time);
    i32_lag += i16_dt;
    i16_prev_time = i16_curr_time;

    //printf("lag: %d, c: %d, p: %d, dt: %d\n", i32_lag, i16_curr_time, i16_prev_time, i16_dt);

    // Should gamemngr handle user input?
    handle_user_input (&player);

    // TODO: test if stuttering occurs on real hardware w/ this whole lag system
    //while (i32_lag >= TICKS_PER_UPDATE)
    //{
      update_game_state ();
      //i32_lag -= TICKS_PER_UPDATE;
    //}
    
    render_screen ();
  }

 return 0;
}

#undef TICKS_PER_UPDATE
#undef CALC_DELTA_TIME
