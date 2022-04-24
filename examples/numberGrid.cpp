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
//    This file is used as an example for the DisplayMenu class.
//    This file contains a demonstration of a screen menu with 4 selectable widgets.
//    This example uses a ST7789 TFT display with the Adafruit GFX library.
//    but the DisplayMenu class can work with other libraries. It is based on the Arduino framework.
//
//      How the screen displays information in this example:
//    _____________________________________________________________________
//    |                                                                    |
//    |  Widget 1: select to change nb 1   Widget 3: select to change nb 3 |
//    |                                                                    |
//    |  Widget 2: select to change nb 2   Widget 4: select to change nb 4 |
//    |                                                                    |
//    ----------------------------------------------------------------------
//    In this example, the four widgets are an example of a modifiable value widget. Other example uses
//    for modifiable widgets may include setting speaker volume, changing a motor speed, etc.
//    The user must first select the widget, then use navigation buttons (i.e. up and down arrows)
//    to change the value. Pressing the select button again will set the new value.
//
//    Widgets are rendered as numbers surrounded by squares, the user can navigate with 5 keys
//
//    To setup this example, you need 5 push buttons, and a ST7789 TFT screen (or other compatible
//    screen by changing the necessary macros, i.e. screen resolution)
//
//***********************************************************************************

#include <Arduino.h>

#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#include "DisplayMenu.h"
#include "DisplayWidget.h"

#define UP_BUTTON_PIN 0
#define LEFT_BUTTON_PIN 1
#define CENTER_BUTTON_PIN 2
#define DOWN_BUTTON_PIN 3
#define RIGHT_BUTTON_PIN 4

#define TFT_CS_PIN 10
#define TFT_DC_PIN 9
#define TFT_RST_PIN 8


// Display constants
#define ST7789_X_PIXEL_NB (240)
#define ST7789_Y_PIXEL_NB (240)


// Display menu configuration
#define MAIN_MENU_WIDGET_NB 4
#define MAIN_MENU_X_WIDGET_NB 2
#define MAIN_MENU_Y_WIDGET_NB 2

// Calculate widget positions
#define LEFT_QUARTER_X_PIX (0)
#define RIGHT_QUARTER_X_PIX (((ST7789_X_PIXEL_NB)/2))

#define TOP_QUARTER_Y_PIX (0)
#define BOTTOM_QUARTER_Y_PIX (((ST7789_Y_PIXEL_NB)/2))

// Set where widgets are (ex: widget 1 is top left)
#define WDG1_X_POS  LEFT_QUARTER_X_PIX
#define WDG1_Y_POS  TOP_QUARTER_Y_PIX

#define WDG2_X_POS  LEFT_QUARTER_X_PIX
#define WDG2_Y_POS  BOTTOM_QUARTER_Y_PIX

#define WDG3_X_POS  RIGHT_QUARTER_X_PIX
#define WDG3_Y_POS  TOP_QUARTER_Y_PIX

#define WDG4_X_POS  RIGHT_QUARTER_X_PIX
#define WDG4_Y_POS  BOTTOM_QUARTER_Y_PIX

// Dimensions
#define WDG_SQUARE_LEN  (ST7789_X_PIXEL_NB/2 -1)  // squares are half the screen -1 so they do not overlap
#define WDG_TEXT_TO_SQUARE_OFFSET (WDG_SQUARE_LEN/2)  // print numbers approximately in the center of their squares

// Global objects

DisplayMenu menu;
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS_PIN, TFT_DC_PIN, TFT_RST_PIN);

int nbOne = 1;
int nbTwo = 1;
int nbThree = 1;
int nbFour = 1;

DisplayWidget nbMenuWidgets[MAIN_MENU_WIDGET_NB] = {
    DisplayWidget(&nbOne, 1, 20, 0, WDG1_X_POS, WDG1_Y_POS),
    DisplayWidget(&nbTwo, 1, 20, 0, WDG2_X_POS, WDG2_Y_POS),
    DisplayWidget(&nbThree, 1, 20, 0, WDG3_X_POS, WDG3_Y_POS),
    DisplayWidget(&nbFour, 1, 20, 0, WDG4_X_POS, WDG4_Y_POS)
};

/***************************************************************************/
/*!
    @brief Prints the menu page with the help of ScreenMenu to manage
     widgets, order and color
    @param none
*/
/***************************************************************************/
void passToPrintNextWidget(bool isFirstWidget) {
  if(isFirstWidget) {
    menu.startPrint();
  } else {
    menu.nextPrint(); // called to tell the menu that we are printing the next widget
  } 
  tft.setTextColor(menu.getPrintColor(), menu.getBackgroundColor());
  tft.setCursor(menu.getPrintX() + WDG_TEXT_TO_SQUARE_OFFSET, menu.getPrintY() + WDG_TEXT_TO_SQUARE_OFFSET);
  tft.drawRect(menu.getPrintX(), menu.getPrintY(), WDG_SQUARE_LEN, WDG_SQUARE_LEN, menu.getPrintColor());
}

void printNumbersPage()
{
  passToPrintNextWidget(1);
  tft.println(nbOne + String(" "));

  passToPrintNextWidget(0);
  tft.println(nbTwo + String(" "));

  passToPrintNextWidget(0);
  tft.println(nbThree + String(" "));

  passToPrintNextWidget(0);
  tft.println(nbFour + String(" "));
}

void setup()
{
  pinMode(CENTER_BUTTON_PIN, INPUT);
  pinMode(RIGHT_BUTTON_PIN, INPUT);
  pinMode(LEFT_BUTTON_PIN, INPUT);
  pinMode(DOWN_BUTTON_PIN, INPUT);
  pinMode(UP_BUTTON_PIN, INPUT);

  // initialize menu
  menu.setColors(ST77XX_WHITE, ST77XX_ORANGE, ST77XX_GREEN, ST77XX_BLACK); // sets the desired colors for the widgets and menu background
  menu.setDisplayedWidgets(nbMenuWidgets, MAIN_MENU_X_WIDGET_NB, MAIN_MENU_Y_WIDGET_NB);

  // initialize screen
  tft.init(ST7789_X_PIXEL_NB, ST7789_Y_PIXEL_NB, SPI_MODE2); // Init ST7789 display 240x240 pixel
  tft.setRotation(1);
  tft.fillScreen(menu.getBackgroundColor());
  tft.setTextWrap(true);
  tft.setTextSize(3);

  printNumbersPage();
}

void loop()
{

  // check if buttons were pressed
  if (digitalRead(CENTER_BUTTON_PIN) == LOW)
  {
    menu.interact();
  }

  if (digitalRead(DOWN_BUTTON_PIN) == LOW)
  {
    menu.moveDown();
  }

  if (digitalRead(UP_BUTTON_PIN) == LOW)
  {
    menu.moveUp();
  }

  if (digitalRead(LEFT_BUTTON_PIN) == LOW)
  {
    menu.moveLeft();
  }

  if (digitalRead(RIGHT_BUTTON_PIN) == LOW)
  {
    menu.moveRight();
  }

  delay(50); // artificially slow button response

  // reprint menu page if it needs to be updated
  if (menu.isChanged())
  {
    printNumbersPage();
  }
}