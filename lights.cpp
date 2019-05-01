#include "lights.h"
#include <OctoWS2811.h>


const int ledsPerStrip = 60;

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];

const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);


unsigned int h2rgb(unsigned int v1, unsigned int v2, unsigned int hue)
{
	if (hue < 60) return v1 * 60 + (v2 - v1) * hue;
	if (hue < 180) return v2 * 60;
	if (hue < 240) return v1 * 60 + (v2 - v1) * (240 - hue);
	return v1 * 60;
}


int makeColor(unsigned int hue, unsigned int saturation, unsigned int lightness)
{
	unsigned int red, green, blue;
	unsigned int var1, var2;

	if (hue > 359) hue = hue % 360;
	if (saturation > 100) saturation = 100;
	if (lightness > 100) lightness = 100;

	// algorithm from: http://www.easyrgb.com/index.php?X=MATH&H=19#text19
	if (saturation == 0) {
		red = green = blue = lightness * 255 / 100;
	} else {
		if (lightness < 50) {
			var2 = lightness * (100 + saturation);
		} else {
			var2 = ((lightness + saturation) * 100) - (saturation * lightness);
		}
		var1 = lightness * 200 - var2;
		red = h2rgb(var1, var2, (hue < 240) ? hue + 120 : hue - 240) * 255 / 600000;
		green = h2rgb(var1, var2, hue) * 255 / 600000;
		blue = h2rgb(var1, var2, (hue >= 120) ? hue - 120 : hue + 240) * 255 / 600000;
	}
	return (red << 16) | (green << 8) | blue;
}

void lights_setup() {
  pinMode(1, OUTPUT);
  digitalWrite(1, HIGH);
  digitalWrite(1, LOW);
  leds.begin();
}

void paint(int position, int color, int lightness, int decay) 
{
  int black = makeColor(0, 0, 0);
  int colors[decay];
  for (int x = 0; x < decay; x++) {
    int l = (x == decay - 1)? lightness : 3 * (x + 1) / 2;
    colors[x] = makeColor(color, 100, l);
  }

  for (int x = 0; x < ledsPerStrip; x++) {
    if (x < position) {
      leds.setPixel(x, black);
    } else if (x < position + decay) {
      leds.setPixel(x, colors[x - position]);
    } else {
      leds.setPixel(x, black);
    }
  }
  leds.show();
}

