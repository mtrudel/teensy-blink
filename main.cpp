#include <wiring.h>
#include <core_pins.h>
#include <usb_dev.h>
#include <usb_serial.h>
#include "encoder.h"
#include "lights.h"

int position = 0;
int color = 140;

void handle_1(int increment) {
  position += increment;
  position = (60 + position) % 60;
  paint(position, color, 30, 10);
  Serial.print("Position: ");
  Serial.println(position);
}

void handle_2(int increment) {
  color += 2 * increment;
  color = (360 + color) % 360;
  paint(position, color, 30, 10);
  Serial.print("Color: ");
  Serial.println(color);
}

int main() {
  encoder_setup();
  encoder_set_handler_1(handle_1);
  encoder_set_handler_2(handle_2);
  lights_setup();
  paint(position, color, 30, 10);

  Serial.begin(9600);

  for(;;) {
    encoder_process();
  }
}
