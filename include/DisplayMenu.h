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
//    This file contains a high level interface for a buzzer. It lets the
//    user create melodies and feed them into the object without blocking operations.
//    The instance steps every ISR. step delay is specified in the ctor.
//    a melody is initialized with setMelody(). the tone will ring, when stepping
//    is complete, melody is cleared
//    https://github.com/espressif/arduino-esp32/issues/1720
//
// Implementation:
//    Implemented with Arduino.h and tone() to control note frequencies
//    On the ESP32, reimplemented with the ledcWriteTone() function
//
//***********************************************************************************

#ifndef DISPLAY_MENU_H
#define DISPLAY_MENU_H

#include <stdint.h>
#include <string.h>
#include "Arduino.h"
#include "DisplayWidget.h"

#define MAX_SINGLE_AXIS_NB_WIDGETS (12)
#define X_Y_AXES_NB (2)

class DisplayMenu
{
private:

  class _widgetPrinter {
    private:
      uint16_t _target = 0;
      uint16_t _nbWidgets = 0;
    public:
      uint16_t getPrintTarget() { return _target; }

  };
  _widgetPrinter _wdgPrint;
  // mapping widgets
  uint16_t _cursorPos[X_Y_AXES_NB] = {0, 0};

  uint16_t _mapDimensions[X_Y_AXES_NB] = {0, 0}; 

  uint16_t _targetIdx;
  bool _isChanged = false;

  DisplayWidget *_menuWidgets; // pointer to array of widgets for current menu
  bool _isEditingTarget;

  // usage settings
  bool _editsFromSides;

  // printing widgets
  // class widgetPrinter with curr target (private), nb of widgets, colors, gettarget which is enclosed, 
  uint16_t _currWdgToPrint = 0;   // default printing target for fcts that take a widget as argument

  int16_t _idleColor;
  int16_t _targetColor;
  int16_t _editingColor;
  int16_t _backgroundColor;

  void _moveCursor(uint8_t dim, int amount);
  void _encloseCursor();
  void _updateTarget();

  // widget or void pointer checks widget type when selectTarget called

  void _updateMapDimensions(int x_count, int y_count);

public:
  DisplayMenu(/* args */) {}
  ~DisplayMenu() {}

  // printing
  void startPrint() { _currWdgToPrint = 0; }
  void nextPrint() { if (_currWdgToPrint < getWidgetNb()) {_currWdgToPrint++; } } 

  // navigation and widget editing
  void startEditingTarget() { _isEditingTarget = true; }
  void stopEditingTarget() { _isEditingTarget = false; }
  bool isEditingTarget() { return _isEditingTarget; }

  void flagChange() { _isChanged = true; }
  bool isChanged();

  // need fct that calls a void ptr, check what type of widget, and either edits or activates it.
  // pass an argument of which direction is pressed, could set custom codes for each, and call that
  // fct inside the UI callback from button presses
  // button press --> callback --> this fct (could not be callback directly?) --> widget activation or start edit or move
 
  /***************************************************************************/
  /*!
      @brief  Set the list of widgets associated with current menu page.
      @param wdgList Pointer to the array of widgets for current page
      @param yNbWdg number of widgets on the y axis for current page
      @param xNbWdg number of widgets on the x axis for current page
  */
  /***************************************************************************/
  void setDisplayedWidgets(DisplayWidget *wdgList, uint16_t yNbWdg, uint16_t xNbWdg = 1);

  
  void setColors(uint16_t idleCol, uint16_t targetCol, uint16_t editingCol, uint16_t backgroundCol);


  uint16_t getWidgetColor(int widgetIdx = -1);
  uint16_t getBackgroundColor() {return _backgroundColor; }

  uint16_t getWidgetNb() { return (_mapDimensions[0] * _mapDimensions[1]); }

  /***************************************************************************/
  /*!
      @brief  Set if user edits widgets values from side arrows rather than up
        and down arrows
      @param sidesEdit triggers editing from side buttons if true.
  */
  /***************************************************************************/
  void editFromSides(bool sidesEdit) { _editsFromSides = sidesEdit; }

  //-------------------------------------
  // Navigation functions
  //-------------------------------------

  void moveUp   (int amount = 1);
  void moveDown (int amount = 1);
  void moveRight(int amount = 1);
  void moveLeft (int amount = 1);
  void activateTarget();
};

#endif
//screen move calls displayMenu move and screen's own refreshPage
// tact press -> ui cb -> screen moveLeft, ... -> displayMenu moveX

//screen print checks displayMenu's maps