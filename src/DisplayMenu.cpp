#include "DisplayMenu.h"

#define X_COORD_INDEX (0)
#define Y_COORD_INDEX (1)

//#######################################################################
// Private functions
//#######################################################################

void DisplayMenu::_encloseCursor()
{
    if (_cursorPos[X_COORD_INDEX] > (_mapDimensions[X_COORD_INDEX] - 1))
    {
        _cursorPos[X_COORD_INDEX] = 0;
    }
    if (_cursorPos[Y_COORD_INDEX] > (_mapDimensions[Y_COORD_INDEX] - 1))
    {
        _cursorPos[Y_COORD_INDEX] = 0;
    }
}

void DisplayMenu::_updateTarget()
{
    _targetIdx = ((_mapDimensions[Y_COORD_INDEX]) * _cursorPos[X_COORD_INDEX]) + _cursorPos[Y_COORD_INDEX];
}

void DisplayMenu::_moveCursor(uint8_t dim, int amount) {
    _cursorPos[dim] += amount;
    _encloseCursor();
    _updateTarget();
    delay(1); // for debug
}

void DisplayMenu::_updateMapDimensions(int x_count, int y_count) {
    _mapDimensions[X_COORD_INDEX] = x_count;
    _mapDimensions[Y_COORD_INDEX] = y_count;
    
    // reset navigation
    _targetIdx = 0;
    _cursorPos[X_COORD_INDEX] = 0;
    _cursorPos[Y_COORD_INDEX] = 0;
    _stopEditingTarget(); // leave editing mode, safeguard code

    // reset printing
    _currWdgToPrint = 0; 
}

//#######################################################################
// Public functions
//#######################################################################

bool DisplayMenu::isChanged()
{
  if (_isChanged)
  {
    _isChanged = false;
    return true;
  }
  else
  {
    return false;
  }
}

uint16_t DisplayMenu::getWidgetColor(uint16_t widgetIdx) {
  if (widgetIdx >= getWidgetNb()) {
    return 0;
  }

  if(_targetIdx == widgetIdx) {
    if (_isEditingTarget)
    {
      return _editingColor;
    } else {
      return _targetColor;
    }
  }
  return _idleColor;
}

uint16_t DisplayMenu::getPrintColor() 
{
  return getWidgetColor(_currWdgToPrint);
}

int DisplayMenu::getPrintX() 
{
  if (_menuWidgets == NULL) {
    return -1;
  }
  return _menuWidgets[_currWdgToPrint].getXPostion();
}

int DisplayMenu::getPrintY() 
{
  if (_menuWidgets == NULL) {
    return -1;
  }
  return _menuWidgets[_currWdgToPrint].getYPostion();
}

uint16_t DisplayMenu::getTargetWidgetColor() 
{
  return getWidgetColor(_targetIdx);
}

void DisplayMenu::setDisplayedWidgets(DisplayWidget *wdgList, uint16_t yNbWdg, uint16_t xNbWdg)
{
  _menuWidgets = wdgList;
  _updateMapDimensions(xNbWdg, yNbWdg);
  
}

void DisplayMenu::setColors(uint16_t idleCol, uint16_t targetCol, uint16_t editingCol, uint16_t backgroundCol)
{
  _idleColor = idleCol;
  _targetColor = targetCol;
  _editingColor = editingCol;
  _backgroundColor = backgroundCol;
}

//-------------------------------------
// Navigation functions
//-------------------------------------

void DisplayMenu::moveUp(int amount /* = 1 */)
{
    if (_menuWidgets == NULL)
    return;
  _isChanged = true;
  if (_isEditingTarget && !_editsFromSides)
  {
    _menuWidgets[_targetIdx].increment();
  }
  else if (!_isEditingTarget)
  {
    _moveCursor(Y_COORD_INDEX, -amount);
  }
}

void DisplayMenu::moveDown(int amount /* = 1 */)
{
    if (_menuWidgets == NULL)
    return;
  _isChanged = true;
  if (_isEditingTarget && !_editsFromSides)
  {
    _menuWidgets[_targetIdx].decrement();
  }
  else if (!_isEditingTarget)
  {
    _moveCursor(Y_COORD_INDEX, amount);
  }
}

void DisplayMenu::moveLeft(int amount /* = 1 */)
{
    if (_menuWidgets == NULL)
    return;
  _isChanged = true;
  if (_isEditingTarget && _editsFromSides)
  {
    _menuWidgets[_targetIdx].decrement();
  }
  else if (!_isEditingTarget)
  {
    _moveCursor(X_COORD_INDEX, -amount);
  }
}

void DisplayMenu::moveRight(int amount /* = 1 */)
{
    if (_menuWidgets == NULL)
    return;
  _isChanged = true;
  if (_isEditingTarget && _editsFromSides)
  {
    _menuWidgets[_targetIdx].increment();
  }
  else if (!_isEditingTarget)
  {
    _moveCursor(X_COORD_INDEX, amount);
  }
}

void DisplayMenu::interact()
{
  if (_menuWidgets == NULL)
    return; 

  _isChanged = true;
  if (_menuWidgets[_targetIdx].is_editable())
  {
    if(!_isEditingTarget) {
      _startEditingTarget();
    } else {
      _stopEditingTarget();
    }
  }
  else
  {
    _menuWidgets[_targetIdx].activate();
  }
}
