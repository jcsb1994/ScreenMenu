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
//    This file contains a demonstration of a screen menu with 2 selectable widgets.
//    This example uses a ST7789 TFT display with the TFT_eSPI.
//    but the DisplayMenu class can work with other libraries. It is based on the Arduino framework.
//
//      How the screen displays information in this example:
//    ______________________________________________
//    |                                             |
//    |  Widget 1: select to change nb of dancers   |
//    |                                             |
//    |  Widget 2: select to start dancing          |
//    |                                             |
//    -----------------------------------------------
//    In this example, widget 1 is an example of a modifiable value widget. Other example uses
//    for modifiable widgets may include setting speaker volume, changing a motor speed, etc.
//    The user must first select the widget, then use navigation buttons (i.e. up and down arrows)
//    to change the value. Pressing the select button again will set the new value.
//    Widget 2 is an example of an action triggering widget. It triggers a function call rather than
//    modifying a value setting. This Can be used to change menu page, start an action, etc.
//
//    The widgets are written in white on the screen. The user can move with a button (next button) to select
//    the widgets and press a second button (select button) to select them. In this example, targeted widgets
//    are magenta, and modifiable widgets will turn green when selected.
//
//    To setup this example, you need 2 buttons, and a TFT screen
//
//***********************************************************************************


#include <Arduino.h>
#include "TFT_eSPI.h"

#include "DisplayMenu.h"
#include "DisplayBitmap.h"
#include "DisplayWidget.h"

#define NEXT_BUTTON_PIN 3
#define ENTER_BUTTON_PIN 4
#define DANCE_MENU_WIDGET_NB 2

// Global objects

DisplayMenu menu;

TFT_eSPI tft;

// screen menu widgets
int nbDancers = 1;

void startDancing() {
  tft.setCursor(0, 150);
  tft.setTextColor(TFT_WHITE, menu.getBackgroundColor());
  tft.print(nbDancers);
  tft.println(" dancers dancing!");
  delay(1000);
  tft.fillScreen(menu.getBackgroundColor());
}

DisplayWidget danceMenuWidgets[DANCE_MENU_WIDGET_NB] = {
  DisplayWidget(&nbDancers, 1, 20), //check to replace pointer with a void pointer, but needs to understand if is a fct ptr
  DisplayWidget(startDancing)
};

/***************************************************************************/
/*!
    @brief Prints the menu page with the help of ScreenMenu to manage
     widgets, order and color
    @param none
*/
/***************************************************************************/
void printDancingPage() {
  tft.setCursor(0, 50);

  menu.startPrint();
  tft.setTextColor(menu.getPrintColor(), menu.getBackgroundColor());
  tft.print("Dancers: ");
  tft.print(nbDancers);
  tft.println("  "); // fills smaller numbers

  menu.nextPrint(); // called to tell the menu that we are printing the next widget
  tft.setTextColor(menu.getPrintColor(), menu.getBackgroundColor()); 
  tft.println("Dance!");
}


void setup() {
  pinMode(ENTER_BUTTON_PIN, INPUT);
  pinMode(NEXT_BUTTON_PIN, INPUT);
  
  // initialize menu
  menu.setColors(TFT_WHITE, TFT_MAGENTA, TFT_RED, TFT_BLACK); // sets the desired colors for the widgets and menu background
  menu.setDisplayedWidgets(danceMenuWidgets, DANCE_MENU_WIDGET_NB);

  // initialize screen
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(menu.getBackgroundColor());
  tft.setTextWrap(true);  
  tft.setTextSize(3);

  printDancingPage();
}

void loop() {

  // check if buttons were pressed
  if (digitalRead(ENTER_BUTTON_PIN) == LOW) { 
    menu.interact();
  }

  if (digitalRead(NEXT_BUTTON_PIN) == LOW) {
      menu.moveDown();
  }

  // reprint menu page if it needs to be updated
  if(menu.isChanged()) 
  {
    printDancingPage();
  }
}