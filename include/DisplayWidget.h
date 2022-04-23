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
//    This file contains a class for a widget object for embedded UI. 
//    Widgets are the primary elements in a user interface (see Qt for example).
//    User can interact with widgets when selected on a UI screen, to navigate
//    within a menu, changes settings, etc.
// Implementation:
//    Implemented with Arduino.h, can be used with various TFT screens, can operate
//    with the Adafruit_GFX library, TFT_eSPI, ...
//
//***********************************************************************************

#ifndef DISPLAY_WIDGET_H
#define DISPLAY_WIDGET_H

#include "stdint.h"

class DisplayWidget
{
public:
  /**********************************************************************/
  /*!
    @brief  Ctor for non modifiable widgets, that trigger an action when pressed
    @param  activation_fct function to run when widget is pressed
  */
  /**********************************************************************/
  DisplayWidget(void(activation_fct)(), int xPos = -1, int yPos = -1) : _value(NULL)
  {
    setPosition(xPos, yPos);
    _activationFct = activation_fct;
    _isEditable = false;
  }

  /**********************************************************************/
  /*!
    @brief  Ctor for non modifiable widgets, that trigger an action with param when pressed.
    Useful when same fct with diff parameter is called for multiple widgets.
    @param  activation_fct function to run when widget is activated
    @param  paramValue     param to pass to the function when widget is activated
  */
  /**********************************************************************/
  DisplayWidget(void(activation_fct)(int), int paramValue) : _value(NULL)
  {
    _activationParam = paramValue;
    _paramActivationFct = activation_fct;
    _isEditable = false;
  }

  /**********************************************************************/
  /*!
    @brief  Ctor for modifiable widgets, that contain a value that can be inc/decremented
    @param  displayedValue variable to link to the widget
    @param  incrementAmount Numeric amount by which the displayed value is changed when edited
  */
  /**********************************************************************/
  DisplayWidget(void *displayedValue, unsigned int incrementAmount, int valueCeiling, int valueFloor = 0, int xPos = -1, int yPos = -1)
      : _value(displayedValue), _incrementSize(incrementAmount),
        _valueCeiling(valueCeiling), _valueFloor(valueFloor)
  {
    setPosition(xPos, yPos);
    _isEditable = true;
  }

  ~DisplayWidget() {}
  bool is_editable() { return _isEditable; }
  void activate() {
    if (_isEditable) {
      return;
    }
    if(_activationFct != NULL) 
      _activationFct();
    else if(_paramActivationFct != NULL) 
      _paramActivationFct(_activationParam);      
  }
  void increment()
  {
    *(int*)_value += _incrementSize;
    if (*(int*)_value > _valueCeiling)
      *(int*)_value = _valueFloor;
  }
  void decrement()
  {
    *(int*)_value -= _incrementSize;
    if (*(int*)_value < _valueFloor)
      *(int*)_value = _valueCeiling;
  }

  void setPosition(int xPos, int yPos) {
    if ((xPos < 0) || (yPos < 0)) {
      return;
    }
    _xPos = xPos;
    _yPos = yPos;
  }

  int getXPostion() {return _xPos; }
  int getYPostion() {return _yPos; }

private:
  // widget position on the display
  int _xPos = -1;
  int _yPos = -1;

  // widgets that store a changeable value
  void *const _value; //  You can not store a function pointer in a void * pointer. This causes UB.
  unsigned int _incrementSize;
  int _valueCeiling;
  int _valueFloor;

  // widgets that trigger an action
  bool _isEditable = 0;
  int _activationParam = 0;
  void (*_activationFct)() = NULL;   // Will need to have a single fct pointer for any parameters/return
  void (*_paramActivationFct)(int) = NULL;
  
};

#endif