#include "encoder.h"
#include "Encoder.h"

Encoder knob1(9, 10);
Encoder knob2(22, 23);

long position1  = -999;
long position2 = -999;

void (*handler_1)(int);
void (*handler_2)(int);

void encoder_setup() {
}

void encoder_set_handler_1(void (* handler)(int)) {
  handler_1 = handler;
}

void encoder_set_handler_2(void (* handler)(int)) {
  handler_2 = handler;
}

void encoder_process() {
 long new1, new2;
  new1 = knob1.read();
  new2 = knob2.read();
  if (new1 != position1 && handler_1) {
    handler_1(new1 - position1);
  }
  if (new2 != position2 && handler_2) {
    handler_2(new2 - position2);
  }
  position1 = new1;
  position2 = new2;
}
