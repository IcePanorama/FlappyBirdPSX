/* Don't touch! CC seems really particular about the order of these. */
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <libetc.h>
/******************/

#include "compnts/wiframe.h"

#ifdef DEBUG_BUILD
  #include <stdio.h>
  #include <assert.h>
#endif /* DEBUG_BUILD */

struct WireframeCompnt_s
{
  uint8_t u8_parent_id;
  LINE_F2 wires[WIFRAME_MAX_NUM_WIRES];
};

WireframeCompnt_t wfc_wireframe_pool[(WIFRAME_MAX_NUM_WIREFRAMES)];
uint8_t u8_wfc_num_wireframes;

void
wfc_init_wireframe_compnt_pool (void)
{
  memset (wfc_wireframe_pool, 0,
          sizeof (WireframeCompnt_t) * (WIFRAME_MAX_NUM_WIREFRAMES));
  u8_wfc_num_wireframes = 0;
}

void
wfc_create_new_wireframe (uint8_t u8_id)
{
#ifdef DEBUG_BUILD
  assert((u8_wfc_num_wireframes + 1) < (WIFRAME_MAX_NUM_WIREFRAMES));
#endif /* DEBUG_BUILD */

  wfc_wireframe_pool[u8_wfc_num_wireframes].u8_parent_id = u8_id;
  u8_wfc_num_wireframes++;
}
