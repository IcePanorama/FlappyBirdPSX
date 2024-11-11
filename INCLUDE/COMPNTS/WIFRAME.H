#ifndef _WIREFRAME_COMPONENT_H_
#define _WIREFRAME_COMPONENT_H_

#define WIFRAME_MAX_NUM_WIRES (5)
#define WIFRAME_MAX_NUM_WIREFRAMES (32)

#include "sys/fb_ints.h"

//TODO: change other data types to forward declarations!
typedef struct WireframeCompnt_s WireframeCompnt_t;


extern WireframeCompnt_t wfc_wireframe_pool[(WIFRAME_MAX_NUM_WIREFRAMES)];
extern uint8_t u8_wfc_num_wireframes;

void wfc_init_wireframe_compnt_pool (void);
void wfc_create_new_wireframe (uint8_t u8_id);

/*
void csc_init_col_shape_compnt_pool (void);
void csc_create_new_col_shape (uint8_t u8_id);
void csc_destroy_col_shape (uint8_t u8_id);
ColShapeCompnt_t *get_col_shape_with_id (uint8_t u8_id);
*/

#endif /* _WIREFRAME_COMPONENT_H_ */
