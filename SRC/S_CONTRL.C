#include "S_CONTRL.H"

void s_init_controllers(void) {
  PadInitDirect(controller_1, controller_2);
  PadStartCom();
}
