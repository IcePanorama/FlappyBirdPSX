#ifndef _WIREFRAME_COMPONENT_H_
#define _WIREFRAME_COMPONENT_H_

// FIXME: should this first define be in the .c file instead?
#define WIFRAME_NUM_WIRES (5)
#define WIFRAME_MAX_NUM_WIREFRAMES (32)

/* Don't touch! CC seems really particular about the order of these. */
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <libetc.h>
/******************/

#include "sys/fb_ints.h"

typedef struct WireframeCompnt_s
{
  uint8_t u8_parent_id;
  uint8_t u8_width;
  uint8_t u8_height;
  LINE_F2 wires[WIFRAME_NUM_WIRES];
} WireframeCompnt_t;

extern WireframeCompnt_t wfc_wireframe_pool[(WIFRAME_MAX_NUM_WIREFRAMES)];
extern uint8_t u8_wfc_num_wireframes;

void wfc_init_wireframe_compnt_pool (void);
void wfc_create_new_wireframe (uint8_t u8_id);
void wfc_destroy_wireframe (uint8_t u8_id);
WireframeCompnt_t *wfc_get_wireframe_with_id (uint8_t u8_id);
void update_wireframe_xy (WireframeCompnt_t *w, int16_t i16_left_x,
                          int16_t i16_right_x, int16_t i16_top_y,
                          int16_t i16_bot_y);

#endif /* _WIREFRAME_COMPONENT_H_ */
