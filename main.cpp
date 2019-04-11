#include <wiring.h>
#include <core_pins.h>
#include <usb_dev.h>
#include <usb_serial.h>
#include "encoder.h"
#include "lights.h"

int brightness = 30;
int color = 140;

void handle_1(int increment) {
  brightness += increment;
  brightness = min(max(brightness, 0), 255);
  lights_setup(brightness);
  paint_color(color);
  Serial.print("Brightness: ");
  Serial.println(brightness);
}

void handle_2(int increment) {
  color += 2 * increment;
  color = (180 + color) % 180;
  paint_color(color);
  Serial.print("Offset: ");
  Serial.println(color);
}

int main() {
  encoder_setup();
  encoder_set_handler_1(handle_1);
  encoder_set_handler_2(handle_2);
  lights_setup(brightness);
  paint_color(color);

  Serial.begin(9600);

  for(;;) {
    encoder_process();
  }
}
