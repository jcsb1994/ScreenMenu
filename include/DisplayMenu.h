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
//    This file contains a high level interface for a user menu in embedded UI.
//
// Implementation:
//    Implemented with Arduino.h, can be used with various TFT screens, can operate
//    with the Adafruit_GFX library, TFT_eSPI, ...
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
public:
  DisplayMenu() {}
  ~DisplayMenu() {}

  /***************************************************************************/
  /*!
      @brief Move the cursor up in the UI screen
      @param amount the amount of boxes (widgets) to move up
  */
  /***************************************************************************/
  void moveUp   (int amount = 1);
  
  /***************************************************************************/
  /*!
      @brief Move the cursor down in the UI screen
      @param amount the amount of boxes (widgets) to move down
  */
  /***************************************************************************/
  void moveDown (int amount = 1);
  
  /***************************************************************************/
  /*!
      @brief Move the cursor right in the UI screen
      @param amount the amount of boxes (widgets) to move right
  */
  /***************************************************************************/
  void moveRight(int amount = 1);
  
  /***************************************************************************/
  /*!
      @brief Move the cursor left in the UI screen
      @param amount the amount of boxes (widgets) to move left
  */
  /***************************************************************************/
  void moveLeft (int amount = 1);

  /***************************************************************************/
  /*!
      @brief Interact with the currently targeted widget on the menu screen. 
      Toggle edit mode for editable widgets, or activate the effect of activated
      widgets.
      @param none
  */
  /***************************************************************************/
  void interact();

  bool isEditingTarget() { return _isEditingTarget; }

  /***************************************************************************/
  /*!
      @brief Set the flag that tell the menu object that something changed
      and it needs to refresh display
      @param none
  */
  /***************************************************************************/
  void flagChange() { _isChanged = true; }
  bool isChanged();

  /***************************************************************************/
  /*!
      @brief Resets a counter that counts which widget we are currently printing
      when printing a menu page. must call nextPrint() between each subsequent
      @param none
  */
  /***************************************************************************/
  void startPrint() { _currWdgToPrint = 0; }
  void nextPrint() { if (_currWdgToPrint < getWidgetNb()) {_currWdgToPrint++; } } 

  void setColors(uint16_t idleCol, uint16_t targetCol, uint16_t editingCol, uint16_t backgroundCol);
  uint16_t getWidgetColor(int widgetIdx = -1);
  uint16_t getBackgroundColor() {return _backgroundColor; }

  uint16_t getWidgetNb() { return (_mapDimensions[0] * _mapDimensions[1]); }

  // need fct that calls a void ptr, check what type of widget, and either edits or activates it.
  // pass an argument of which direction is pressed, could set custom codes for each, and call that
  // fct inside the UI callback from button presses
  // button press --> callback --> this fct (could not be callback directly?) --> widget activation or start edit or move
 
  /***************************************************************************/
  /*!
      @brief Set the list of widgets associated with current menu page.
      @param wdgList Pointer to the array of widgets for current page
      @param yNbWdg number of widgets on the y axis for current page
      @param xNbWdg number of widgets on the x axis for current page
  */
  /***************************************************************************/
  void setDisplayedWidgets(DisplayWidget *wdgList, uint16_t yNbWdg, uint16_t xNbWdg = 1);

  /***************************************************************************/
  /*!
      @brief Set if user edits widgets values from side arrows rather than up
        and down arrows
      @param sidesEdit triggers editing from side buttons if true.
  */
  /***************************************************************************/
  void setEditFromSides(bool sidesEdit) { _editsFromSides = sidesEdit; }

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
  bool _isChanged = false;      // marks when something changed on the menu and it needs refresh

  DisplayWidget *_menuWidgets;  // pointer to array of widgets for current menu
  bool _isEditingTarget;

  // usage settings
  bool _editsFromSides;         // left and right buttons edit selected widget instead of up and down buttons

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

    /***************************************************************************/
  /*!
      @brief For editable widgets: Set the flag that tell the menu object 
      that the currently targeted widget is selected for edit
      @param none
  */
  /***************************************************************************/
  void _startEditingTarget() { _isEditingTarget = true; }
  void _stopEditingTarget() { _isEditingTarget = false; }

  // widget or void pointer checks widget type when selectTarget called

  void _updateMapDimensions(int x_count, int y_count);
};

#endif
//screen move calls displayMenu move and screen's own refreshPage
// tact press -> ui cb -> screen moveLeft, ... -> displayMenu moveX

//screen print checks displayMenu's maps