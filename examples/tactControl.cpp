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
//    This file demonstrates the same example as the SimpleMenu example in this folder
//    but adds the tact library by jcsb1994 to facilitate button usage
//
//***********************************************************************************


#include <Arduino.h>
#include "TFT_eSPI.h"

#include "DisplayMenu.h"
#include "DisplayBitmap.h"
#include "DisplayWidget.h"

// This example includes the tact library by jcsb1994 to show the 2 libraries in conjunction
#include "tact.h"

#define NEXT_BUTTON_PIN 0
#define ENTER_BUTTON_PIN 36
#define DANCE_MENU_WIDGET_NB 2

// Global objects
tact nextButton = tact(NEXT_BUTTON_PIN);
tact enterButton = tact(ENTER_BUTTON_PIN);

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
  if (enterButton.poll() == DFLT_TACT_SHORT_PRESS_CODE) { 
    menu.interact();
  }

  if (nextButton.poll() == DFLT_TACT_SHORT_PRESS_CODE) {
      menu.moveDown();
  }

  // reprint menu page if it needs to be updated
  if(menu.isChanged()) 
  {
    printDancingPage();
  }
}