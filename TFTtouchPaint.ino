#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>
#include <Wire.h>      // this is needed even tho we aren't using it
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>

#include <Adafruit_TSC2046.h>
#define TSC_CS         10  // chip select pin
#define TS_RESISTANCE 400  // For a 400Ω resistance across X- and X+.
Adafruit_TSC2046 touchscreen;

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 11
#define TFT_CS 12
#define TFT_RST 16
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

#define TS_MINX 200
#define TS_MINY 305
#define TS_MAXX 3700
#define TS_MAXY 3950

#define DISP_MAXY 320
#define DISP_MAXX 240

#define BOXSIZE 40
#define PENRADIUS 3
int oldcolor, currentcolor;
uint16_t drawColor = ILI9341_WHITE;

void setup() {
  Serial.begin(115200);
  // Wait till serial connection opens for native USB devices
  while (!Serial) {
    delay(10);
  }
 // touch screen setup
  touchscreen.begin(TSC_CS, &SPI, TS_RESISTANCE);
  touchscreen.enableInterrupts(true);

  tft.begin();
  tft.fillScreen(ILI9341_WHITE);

  setColorMenu();
//
//  tft.fillCircle(0, 0, 10, ILI9341_WHITE);
//  tft.fillCircle(200, 0, 10, ILI9341_GREEN);
//  tft.fillCircle(0, 200, 10, ILI9341_BLUE);
//  tft.fillCircle(200, 200, 10, ILI9341_YELLOW);
}
int touch_intx(float px){
  return int(((px - 275) * 0.0685) + 0.5);
}
int touch_inty(float py){
  return DISP_MAXY - int(((py - 200) * 0.0855) + 0.5);
}

void setColorMenu(){
  // make the color selection boxes
  tft.fillRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_RED);
  tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9341_YELLOW);
  tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, ILI9341_GREEN);
  tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, ILI9341_CYAN);
  tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, ILI9341_BLUE);
  tft.drawRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, ILI9341_BLACK);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(BOXSIZE*5+8, 13); // テキストの開始位置を設定
  tft.print("Clear");
}

//void displayTouchPoint(TSPoint point) {
//  Serial.print("X:");
//  Serial.print(touch_intx(point.x));
//  Serial.print("\tY:");
//  Serial.print(touch_inty(point.y));
//  Serial.print("\n");
//}

void clearDraw() {
  tft.fillRect(0, BOXSIZE, DISP_MAXX, DISP_MAXY - BOXSIZE, ILI9341_WHITE);
}

void touchedMenu(int touch_x) {
    setColorMenu();
    if( touch_x < BOXSIZE ) {
      drawColor = ILI9341_RED;
      tft.drawRect(0, 0, BOXSIZE, BOXSIZE, ILI9341_BLACK);
    }
    else if( touch_x < BOXSIZE * 2 ) {
      drawColor = ILI9341_YELLOW;
      tft.drawRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, ILI9341_BLACK);
    }
    else if( touch_x < BOXSIZE * 3 ) {
      drawColor = ILI9341_GREEN;
      tft.drawRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, ILI9341_BLACK);
    }
    else if( touch_x < BOXSIZE * 4 ) {
      drawColor = ILI9341_CYAN;
      tft.drawRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, ILI9341_BLACK);
    }
    else if( touch_x < BOXSIZE * 5 ) {
      drawColor = ILI9341_BLUE;
      tft.drawRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, ILI9341_BLACK); 
    }
    else if( touch_x < BOXSIZE * 6 ) {
      clearDraw();
    }
}

void loop() {
  int touch_x;
  int touch_y;
  delay(20); // Add delay to avoid overloading the serial monitor

  if (touchscreen.isTouched()) {
    TSPoint point = touchscreen.getPoint();
    touch_x = touch_intx(point.x);
    touch_y = touch_inty(point.y);
    
    int radius = 5; // ペンの円の半径
    if(touch_x > 0 && touch_y > BOXSIZE) {
      tft.fillCircle(touch_x, touch_y, radius, drawColor);
    
    }else if(touch_x >= 0 && touch_y <= BOXSIZE) {
      touchedMenu(touch_x);
    }
  }
}
