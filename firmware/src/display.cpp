#include "display.h"
#include <Arduino.h>
#include <Adafruit_SH110X.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeMono18pt7b.h>
#include "debug.h"
#include "bluetooth.h"
#include "battery.h"

// 'charged-screen', 128x64px
const unsigned char epd_bitmap_charged_screen [] PROGMEM = {
	0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 
	0x85, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x85, 
	0xeb, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0xeb, 
	0x9d, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0x9d, 
	0xa3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa3, 
	0x9d, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x9d, 
	0x91, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x91, 
	0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 
	0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 
	0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 
	0x52, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 
	0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 
	0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 
	0x52, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 
	0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 
	0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 
	0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 
	0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 
	0x52, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 
	0x42, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 
	0x42, 0x00, 0x0f, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 
	0x52, 0x00, 0x0f, 0x00, 0x3f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 
	0x34, 0x00, 0x30, 0xc0, 0x7f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 
	0x10, 0x00, 0x30, 0xc0, 0x7f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 
	0x10, 0x00, 0x3c, 0x20, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 
	0x34, 0x00, 0x0c, 0x00, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 
	0x52, 0x00, 0x0c, 0x00, 0xff, 0xfc, 0x01, 0xfe, 0x1f, 0xff, 0xe0, 0x00, 0x07, 0xff, 0x80, 0x52, 
	0x42, 0x00, 0x00, 0x0f, 0xff, 0xfc, 0x03, 0x03, 0xf0, 0x00, 0x3f, 0xff, 0xfc, 0x00, 0x80, 0x42, 
	0x42, 0x00, 0x00, 0x0f, 0xe7, 0xfc, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x42, 
	0x52, 0x00, 0x40, 0x13, 0xcb, 0xf8, 0x02, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x04, 0xc0, 0x52, 
	0x34, 0x00, 0x40, 0x13, 0xc3, 0xf8, 0x02, 0x1e, 0x10, 0x0e, 0x2c, 0x3e, 0x18, 0x04, 0x40, 0x34, 
	0x10, 0x01, 0xc0, 0x0f, 0xe7, 0xe0, 0x02, 0x21, 0x1e, 0x12, 0x12, 0x42, 0x24, 0x7c, 0x40, 0x10, 
	0x10, 0x01, 0xc0, 0x0f, 0xff, 0xe0, 0x02, 0x40, 0x21, 0x22, 0x20, 0x44, 0x7c, 0x88, 0xc0, 0x10, 
	0x34, 0x01, 0x80, 0xc3, 0xff, 0x80, 0x06, 0x40, 0x21, 0x26, 0x20, 0x3c, 0x40, 0x88, 0x80, 0x34, 
	0x52, 0x01, 0x80, 0xc3, 0xff, 0x80, 0x04, 0x3e, 0x22, 0x1b, 0x20, 0x04, 0x38, 0x78, 0x80, 0x52, 
	0x42, 0x00, 0x7f, 0x00, 0x78, 0x1c, 0x04, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x80, 0x42, 
	0x42, 0x00, 0x7f, 0x00, 0x78, 0x1c, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x42, 
	0x52, 0x00, 0x00, 0x0c, 0x00, 0x63, 0x03, 0xfc, 0x1f, 0xff, 0xf0, 0x00, 0x7f, 0xfe, 0x00, 0x52, 
	0x34, 0x00, 0x00, 0x0c, 0x00, 0x63, 0x00, 0x07, 0xf0, 0x00, 0x1f, 0xff, 0xc0, 0x00, 0x00, 0x34, 
	0x10, 0x00, 0x03, 0xf0, 0x81, 0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 
	0x10, 0x00, 0x03, 0xf0, 0x81, 0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 
	0x34, 0x00, 0x0c, 0x00, 0x98, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 
	0x52, 0x00, 0x0c, 0x00, 0x98, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 
	0x42, 0x00, 0x3c, 0x3c, 0x98, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 
	0x42, 0x00, 0x30, 0xf0, 0x86, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 
	0x52, 0x00, 0x30, 0xf0, 0x86, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 
	0x34, 0x00, 0x00, 0xc3, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 
	0x10, 0x00, 0x00, 0xc3, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 
	0x10, 0x00, 0x00, 0x3c, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 
	0x34, 0x00, 0x00, 0x3c, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 
	0x52, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 
	0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 
	0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 
	0x52, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 
	0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 
	0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 
	0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 
	0x85, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x85, 
	0xeb, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0xeb, 
	0x9d, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0x9d, 
	0xa3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa3, 
	0x9d, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x9d, 
	0x91, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x91, 
	0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff
};
// 'charging-screen', 128x64px
const unsigned char epd_bitmap_charging_screen [] PROGMEM = {
	0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 
	0x85, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x85, 
	0xeb, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0xeb, 
	0x9d, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0x9d, 
	0xa3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa3, 
	0x9d, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x9d, 
	0x91, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x91, 
	0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 
	0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 
	0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 
	0x52, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 
	0x42, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 
	0x42, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 
	0x52, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 
	0x34, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 
	0x10, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 
	0x10, 0x0c, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 
	0x34, 0x30, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 
	0x52, 0x30, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 
	0x42, 0x30, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 
	0x42, 0x3f, 0xc3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 
	0x52, 0x3f, 0xc3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 
	0x34, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 
	0x10, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 
	0x10, 0x00, 0x0f, 0x0e, 0x07, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 
	0x34, 0x00, 0x0f, 0x02, 0x3f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 
	0x52, 0x00, 0x30, 0xc4, 0x7f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 
	0x42, 0x00, 0x30, 0xc8, 0x7f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 
	0x42, 0x00, 0x3c, 0x2e, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 
	0x52, 0x00, 0x0c, 0x00, 0xff, 0xfc, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x52, 
	0x34, 0x00, 0x0c, 0x00, 0xff, 0xfc, 0x00, 0x1e, 0x10, 0x0e, 0x2c, 0x3e, 0x03, 0x87, 0xc0, 0x34, 
	0x10, 0x00, 0x00, 0x0f, 0xff, 0xfc, 0x00, 0x21, 0x1e, 0x12, 0x12, 0x42, 0x62, 0x48, 0x40, 0x10, 
	0x10, 0x00, 0x00, 0x0f, 0xff, 0xfc, 0x00, 0x40, 0x21, 0x22, 0x20, 0x44, 0x44, 0x48, 0x80, 0x10, 
	0x34, 0x00, 0x40, 0x13, 0x99, 0xf8, 0x00, 0x40, 0x21, 0x26, 0x20, 0x3c, 0x44, 0x47, 0x80, 0x34, 
	0x52, 0x00, 0x40, 0x13, 0x81, 0xf8, 0x00, 0x3e, 0x22, 0x1b, 0x20, 0x04, 0x24, 0x80, 0x80, 0x52, 
	0x42, 0x01, 0xc0, 0x0f, 0xe7, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x0f, 0x00, 0x42, 
	0x42, 0x01, 0xc0, 0x0f, 0xe7, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 
	0x52, 0x01, 0x80, 0xc3, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 
	0x34, 0x01, 0x80, 0xc3, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 
	0x10, 0x00, 0x7f, 0x00, 0x78, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 
	0x10, 0x00, 0x7f, 0x00, 0x78, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 
	0x34, 0x00, 0x00, 0x0c, 0x00, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 
	0x52, 0x00, 0x00, 0x0c, 0x00, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 
	0x42, 0x00, 0x03, 0xf0, 0x81, 0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 
	0x42, 0x00, 0x03, 0xf0, 0x81, 0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 
	0x52, 0x00, 0x0c, 0x00, 0x98, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 
	0x34, 0x00, 0x0c, 0x00, 0x98, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 
	0x10, 0x00, 0x3c, 0x3c, 0x98, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 
	0x10, 0x00, 0x30, 0xf0, 0x86, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 
	0x34, 0x00, 0x30, 0xf0, 0x86, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 
	0x52, 0x00, 0x00, 0xc3, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 
	0x42, 0x00, 0x00, 0xc3, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 
	0x42, 0x00, 0x00, 0x3c, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 
	0x52, 0x00, 0x00, 0x3c, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 
	0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x34, 
	0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 
	0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 
	0x85, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x85, 
	0xeb, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0xeb, 
	0x9d, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0x9d, 
	0xa3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa3, 
	0x9d, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x9d, 
	0x91, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x91, 
	0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff
};
// 'battery-full', 8x8px
const unsigned char epd_bitmap_battery_full [] PROGMEM = {
	0x18, 0x3c, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x3c
};
// 'battery-empty', 8x8px
const unsigned char epd_bitmap_battery_empty [] PROGMEM = {
	0x18, 0x24, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3c
};
// 'battery-1-3', 8x8px
const unsigned char epd_bitmap_battery_1_3 [] PROGMEM = {
	0x18, 0x24, 0x42, 0x42, 0x42, 0x7e, 0x7e, 0x3c
};
// 'battery-2-3', 8x8px
const unsigned char epd_bitmap_battery_2_3 [] PROGMEM = {
	0x18, 0x24, 0x42, 0x7e, 0x7e, 0x7e, 0x7e, 0x3c
};
// 'charging', 8x8px
const unsigned char epd_bitmap_charging [] PROGMEM = {
	0x00, 0x04, 0x0f, 0x3c, 0x4f, 0x84, 0x40, 0x30
};
// 'bluetooth-disconnected', 8x8px
const unsigned char epd_bitmap_bluetooth_disconnected [] PROGMEM = {
	0x3c, 0x16, 0x82, 0x46, 0x62, 0x51, 0x78, 0x3c
};
// 'bluetooth', 8x8px
const unsigned char epd_bitmap_bluetooth [] PROGMEM = {
	0x3c, 0x76, 0x52, 0x66, 0x66, 0x52, 0x76, 0x3c
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 2304)
const int epd_bitmap_allArray_LEN = 9;
const unsigned char* epd_bitmap_allArray[9] = {
	epd_bitmap_battery_1_3,
	epd_bitmap_battery_2_3,
	epd_bitmap_battery_empty,
	epd_bitmap_battery_full,
	epd_bitmap_bluetooth,
	epd_bitmap_bluetooth_disconnected,
	epd_bitmap_charged_screen,
	epd_bitmap_charging,
	epd_bitmap_charging_screen
};

namespace Display {
  Adafruit_SH1106G display = Adafruit_SH1106G(128, 64, &Wire, 14);

  void init() {
    Wire.begin(12, 13);
    delay(250); // wait for the OLED to power up
    display.begin(0x3C, true);

    // Clear the buffer.
    display.clearDisplay();
    display.display();
    display.setRotation(0);
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0,0);
  }

  void showMass(double rawMass, int x, int y) {
    double mass = rawMass;

    if (mass > -0.1 && mass < 0) {
      mass = 0;
    }

    char massChars[16];
    sprintf(massChars, "%-3.1f", mass);

    String massString(massChars);

    while (massString.length() < 5) {
      massString = " " + massString;
    }

    DEBUG_SERIAL.println(massString);
    
    display.setFont(&FreeMono18pt7b);

    display.setCursor(x, y);
    display.printf(massString.c_str());
    display.setFont();
    display.setCursor(0,16+10);
    //display.printf("sttl: %.1f, dT: %.f\n", scale.getLastSettledReading(), scale.millisBetweenSettledReadings);
  }

  void showTimer(long seconds, double mass) {
    clear();
    display.print("timing: ");
    display.printf("%02d:%02d", seconds / 60, seconds % 60);
    showMass(mass);
    show();
  }

  void showTimerPriming() {
    clear();    
    display.println("timer priming");
    show();    
  }

  void showTimerWaitingForStart(double mass) {
    clear();
    display.println("timer primed");
    showMass(mass);
    show();
  }

  void showBrewCompleteAnimation() {
    for (int i = 0; i < 5; i++) {
      display.clearDisplay();
      display.setCursor(20,30);
      display.println("<<BREW COMPLETE>>");
      display.display();
      delay(500);
      display.clearDisplay();
      display.display();
      delay(500);
    }
  }

  void showBrewStats(double mass, unsigned long duration) {
    display.printf("%-3.1fg in %02dm:%02ds,", mass, duration / 60, duration % 60);
  }

  void showAutoTare() {
    display.clearDisplay();
    display.setCursor(0,12);
    display.setFont(&FreeMono12pt7b);
    display.println("AUTO");
    display.println("TARE");
    display.display();
  }

  void showCalibrationScreen() {
    clear();
    display.println("Calibration mode");
    display.println("Place a 100g mass");
    display.println("Then press B");
    show();
  }

  void showCalibrationCompleteScreen(float factor) {
    clear();
    display.println("Calibrated!");
    display.println("Factor: ");
    display.println(factor);
    show();
  }

  void showCountdown(int seconds) {
    clear();
    display.println(seconds);
    show();
  }

  void showStatusBar() {
    display.setCursor(0, 0);
    display.fillRect(80, 0, 128, 8, SH110X_BLACK);
    
    double batteryValue = Battery::getVoltage();
    display.setCursor(80, 0);
    display.print(batteryValue);
    if (Battery::isCharging()) {
      display.drawBitmap(110, 0, epd_bitmap_charging, 8, 8, SH110X_WHITE);
    } else {
      if (batteryValue > 0.9) {
        display.drawBitmap(110, 0, epd_bitmap_battery_full, 8, 8, SH110X_WHITE);
      } else if (batteryValue > 0.66) {
        display.drawBitmap(110, 0, epd_bitmap_battery_2_3, 8, 8, SH110X_WHITE);
      } else if (batteryValue > 0.33) {
        display.drawBitmap(110, 0, epd_bitmap_battery_1_3, 8, 8, SH110X_WHITE);
      } else {
        display.drawBitmap(110, 0, epd_bitmap_battery_empty, 8, 8, SH110X_WHITE);
      } 
    }
    
    if (BLE::isDeviceConnected()) {
      display.drawBitmap(120, 0, epd_bitmap_bluetooth, 8, 8, SH110X_WHITE);
    } else {
      display.drawBitmap(120, 0, epd_bitmap_bluetooth_disconnected, 8, 8, SH110X_WHITE);
    }
  }

  void showSleepCharging() {
    clear();
    display.drawBitmap(0, 0, epd_bitmap_charging_screen, 128, 64, SH110X_WHITE);
    display.display();
  }

  void showSleepCharged() {
    clear();
    display.drawBitmap(0, 0, epd_bitmap_charged_screen, 128, 64, SH110X_WHITE);
    display.display();
  }

  void clear() {
    display.clearDisplay();
    display.setCursor(0,0);
  }

  void show() {
    showStatusBar();
    display.display();
  }

  Adafruit_SH1106G getDisplay() {
    return display;
  }
}