#include "encoder.h"
#include <Bounce.h>
#include <core_pins.h>

Bounce pushButton_1 = Bounce(17, 5);
Bounce rotaryA_1 = Bounce(18, 5);
Bounce rotaryB_1 = Bounce(19, 5);

Bounce pushButton_2 = Bounce(11, 5);
Bounce rotaryA_2 = Bounce(10, 5);
Bounce rotaryB_2 = Bounce(9, 5);

void (*handler_1)(int);
void (*handler_2)(int);

void encoder_setup() {
  pinMode(17, INPUT_PULLUP); // Z
  pinMode(18, INPUT_PULLUP); // A
  pinMode(19, INPUT_PULLUP); // B

  pinMode(11, INPUT_PULLUP); // Z
  pinMode(10, INPUT_PULLUP); // A
  pinMode(9, INPUT_PULLUP); // B
}

void encoder_set_handler_1(void (* handler)(int)) {
  handler_1 = handler;
}

void encoder_set_handler_2(void (* handler)(int)) {
  handler_2 = handler;
}

void encoder_process() {
  pushButton_1.update();
  rotaryA_1.update();
  rotaryB_1.update();

  pushButton_2.update();
  rotaryA_2.update();
  rotaryB_2.update();

  if (rotaryA_1.fallingEdge()) {
    handler_1(rotaryB_1.read()? 1 : -1);
  }

  if (pushButton_1.fallingEdge()) {
    handler_1(0);
  }

  if (rotaryA_2.fallingEdge()) {
    handler_2(rotaryB_2.read()? 1 : -1);
  }

  if (pushButton_2.fallingEdge()) {
    handler_2(0);
  }
}
