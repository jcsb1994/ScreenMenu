#ifndef DISPLAY_WIDGET_H
#define DISPLAY_WIDGET_H

#include "stdint.h"

class DisplayWidget
{
private:
  // widgets that store a changeable value
  int *const _value;
  unsigned int _incrementSize;
  int _valueCeiling;
  int _valueFloor;

  // widgets that trigger an action
  bool _isEditable = 0;
  int _activationParam = 0;
  void (*_activationFct)() = NULL;
  void (*_paramActivationFct)(int) = NULL;
public:
  /**********************************************************************/
  /*!
    @brief  Ctor for non modifiable widgets, that trigger an action when pressed
    @param  activation_fct function to run when widget is pressed
  */
  /**********************************************************************/
  DisplayWidget(void(activation_fct)()) : _value(NULL)
  {
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
  DisplayWidget(int *displayedValue, unsigned int incrementAmount = 1, int valueCeiling = 100, int valueFloor = 0)
      : _value(displayedValue), _incrementSize(incrementAmount),
        _valueCeiling(valueCeiling), _valueFloor(valueFloor)
  {
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
    *_value += _incrementSize;
    if (*_value > _valueCeiling)
      *_value = _valueFloor;
  }
  void decrement()
  {
    *_value -= _incrementSize;
    if (*_value < _valueFloor)
      *_value = _valueCeiling;
  }
};

#endif