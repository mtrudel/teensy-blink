#include <wiring.h>
#include <core_pins.h>
#include <usb_dev.h>
#include <usb_serial.h>
#include "encoder.h"
#include "lights.h"

int rate = 10;
int position = 0;
int color = 140;

void handle_1(int increment) {
  rate += increment;
  rate = max(-60, min(60, rate));
  Serial.print("Rate: ");
  Serial.println(rate);
}

void handle_2(int increment) {
  color += 2 * increment;
  color = (360 + color) % 360;
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
    if (rate > 0) {
      position += 1;
      delayMicroseconds(1000000 / rate);
    } else if (rate < 0) {
      position -= 1;
      delayMicroseconds(-1000000 / rate);
    }
    position = (position + 60) % 60;

    paint(position, color, 30, 10);
  }
}
