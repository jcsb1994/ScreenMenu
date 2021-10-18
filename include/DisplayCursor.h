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
//    This file manages a 2 dimensional cursor object for UI screens.
//    Designed so that coordinates 0,0 is top left of the screen.
//
//***********************************************************************************

#ifndef DISPLAY_CURSOR_H
#define DISPLAY_CURSOR_H

#include "stdint.h"

class DisplayCursor
{
private:
  uint8_t _lineHeight = 0;
  uint8_t _returnTab = 0;

public:
  uint8_t x;
  uint8_t y;

/***************************************************************************/
  /*!
    @brief  Ctor for DisplayCursor
    @param  startX x axis pos
    @param  startX yaxis pos. 0 is top of screen
    @param  lineHeight Distance in pixels when calling lineFeed()
    @param  returnTab 
  */
/***************************************************************************/
  DisplayCursor(uint8_t startX = 0, uint8_t startY = 0, uint8_t lineHeight = 0, uint8_t returnTab = 0)
  {
    x = startX;
    y = startY;
    _lineHeight = lineHeight;
    _returnTab = returnTab;
  }
    
/***************************************************************************/
  /*!
    @brief  Set the position of the cursor in pixels on the display screen
    @param  xPos x axis pos
    @param  yPos yaxis pos
  */
/***************************************************************************/
  void setPos(uint8_t xPos, uint8_t yPos)
  {
    x = xPos;
    y = yPos;
  }

/***************************************************************************/
  /*!
    @brief  Set the position of the cursor in pixels on the display screen
    @param  lineHeight height of a line when calling a LF
  */
/***************************************************************************/
  void setLineHeight(uint8_t lineHeight) { _lineHeight = lineHeight; }
  void setReturnTab(uint8_t returnTab)   { _returnTab  = returnTab;  }

/***************************************************************************/
  /*!
    @brief  Carriage return the cursor to returnTab value. 
    @param  addedTab Added value to the predetermined returnTab value;
  */
/***************************************************************************/
  void carriageReturn(uint8_t addedTab = 0)
  {
    x = (_returnTab + addedTab);
  }

/***************************************************************************/
  /*!
    @brief   Move the cursor in the y direction by lineHeight
    @param  addedHeight Added value to the predetermined lineHeight value;
  */
/***************************************************************************/
  void lineFeed(uint8_t addedHeight = 0)
  {
    y += (_lineHeight + addedHeight);
  }

/***************************************************************************/
  /*!
    @brief  Perform a carriage return (CR) and line feed (LF)
    @param  addedTab Added value to the predetermined returnTab value;
    @param  addedHeight Added value to the predetermined lineHeight value;
  */
/***************************************************************************/
  void newLine(uint8_t addedTab = 0, uint8_t addedHeight = 0) {
    carriageReturn(addedTab);
    lineFeed(addedHeight);
  }
};

#endif