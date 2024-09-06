#ifndef _V_VIDEO_H_
#define _V_VIDEO_H_

#define SCREEN_WIDTH (320)
#define SCREEN_HEIGHT (240)

#include "V_SCRBUF.H"

void init_video_subsystem (ScreenBuffer_t *sb);
void display_video (ScreenBuffer_t *sb, POLY_F4 *sprite);

#endif /* _V_VIDEO_H_ */
