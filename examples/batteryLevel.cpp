//***********************************************************************************
// Copyright 2021 jcsb1994
// Written by jcsb1994
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//***********************************************************************************
//
// Description:
//    This file is used as an example for the DisplayBitmap class that is part of this library.
//    This file contains a demonstration of a battery logo that slowly reduces in percentage
//    This example uses a ST7789 TFT display with the TFT_eSPI, and the Arduino framework.
//
//***********************************************************************************

#include <Arduino.h>
#include "TFT_eSPI.h"

// This library
#include "DisplayBitmap.h"

#define TFT_SIDE_PIXEL_MAX (240)
#define MAX_BATTERY_PERCENTAGE (100)


//############################################################
// BITMAPS
//############################################################

#define BATT_JUICE_RECT_MAX_W 22
#define BATT_JUICE_RECT_MAX_H 13

#define BATTERY_BMP_WIDTH 26
#define BATTERY_BMP_HEIGHT 17

// Store the bitmap in FLASH memory i.e. PROGMEM
const unsigned char batteryBmp[] PROGMEM = {
    0x3f, 0xff, 0xf8, 0x00, 0xff, 0xff, 0xfe, 0x00, 0xc0, 0x00, 0x06, 0x00, 0xc0, 0x00, 0x03, 0x00,
    0xc0, 0x00, 0x03, 0x80, 0xc0, 0x00, 0x03, 0xc0, 0xc0, 0x00, 0x00, 0xc0, 0xc0, 0x00, 0x00, 0xc0,
    0xc0, 0x00, 0x00, 0xc0, 0xc0, 0x00, 0x00, 0xc0, 0xc0, 0x00, 0x03, 0xc0, 0xc0, 0x00, 0x03, 0xc0,
    0xc0, 0x00, 0x03, 0x80, 0xc0, 0x00, 0x03, 0x00, 0xe0, 0x00, 0x06, 0x00, 0xff, 0xff, 0xfe, 0x00,
    0x3f, 0xff, 0xf8, 0x00};

DisplayBitmap batteryLogo = DisplayBitmap(BATTERY_BMP_WIDTH, BATTERY_BMP_HEIGHT, batteryBmp);


const unsigned char chargeBmp [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x10, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x0f, 0xf8, 0x00, 
	0x3f, 0xfc, 0x00, 0x00, 0x07, 0xfc, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00
};

DisplayBitmap chargeLogo = DisplayBitmap(BATTERY_BMP_WIDTH, BATTERY_BMP_HEIGHT, chargeBmp);

//############################################################

TFT_eSPI tft;

int8_t battPercentage = 100;
bool charging = false;


/***************************************************************************/
/*!
    @brief Prints the battery animation on screen with the current power level
    @param none
*/
/***************************************************************************/
void printBattery()
{
int rectW = (batteryLogo.width * battPercentage)/100;

//clear battery area by covering it with a black rectangle
tft.fillRect(TFT_SIDE_PIXEL_MAX - batteryLogo.width,
               0,
               batteryLogo.width,
               batteryLogo.height,
               TFT_BLACK);

  // draw battery
  tft.drawBitmap(TFT_SIDE_PIXEL_MAX - batteryLogo.width,
                 0,
                 batteryLogo.bitmap, batteryLogo.width,
                 batteryLogo.height, TFT_WHITE);

  // draw green rectangle with variable size inside the battery bitmap
  tft.fillRect(TFT_SIDE_PIXEL_MAX - batteryLogo.width,
               2,
               rectW,
               BATT_JUICE_RECT_MAX_H,
               TFT_GREEN);

  // draw charging spark logo when battery level is increasing
  if(charging) {
    tft.drawBitmap(TFT_SIDE_PIXEL_MAX - chargeLogo.width,
                  0,
                  chargeLogo.bitmap, chargeLogo.width, 
                  chargeLogo.height, TFT_WHITE);
  }
  Serial.println("Battery level green rectangle width:" + String(rectW));
}

void setup()
{
  Serial.begin(9600);
  Serial.println("hello");

  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(TFT_SIDE_PIXEL_MAX/4, TFT_SIDE_PIXEL_MAX/2);
  tft.print("Battery animation");
}

void loop()
{
  Serial.println("Battery %: " + String(battPercentage));

  printBattery();

  // Change battery level
  if (!charging) {
    battPercentage--;
    if(!battPercentage)
      charging = true;
  } else {
    battPercentage++;
    if(battPercentage >= 100) {
      charging = false;
    }
  }
  
  battPercentage = constrain(battPercentage, 0, 100);

  delay(200);
}