#include <Arduino.h>
#include "TFT_eSPI.h"
#include "DisplayMenu.h"
#include "DisplayBitmap.h"
#include "DisplayWidget.h"

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

// menu printing function
void printDancingPage() {
  menu.startPrint();
  tft.setTextColor(menu.getWidgetColor(), menu.getBackgroundColor());

  tft.setCursor(0, 50);
  tft.print("Dancers: ");
  tft.print(nbDancers);
  tft.println("  "); // fills smaller numbers

  menu.nextPrint();
  tft.setTextColor(menu.getWidgetColor(), menu.getBackgroundColor());

  tft.println("Dance!");
}

// main
void setup() {
  // initialize menu
  menu.setColors(TFT_WHITE, TFT_MAGENTA, TFT_RED, TFT_BLACK);
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
    menu.activateTarget();
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