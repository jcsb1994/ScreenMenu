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
//    This file contains a structure for generating a 2 dimensional bitmap object.
//
//***********************************************************************************

#ifndef DISPLAY_BITMAP_H
#define DISPLAY_BITMAP_H

#include "stdint.h"

struct DisplayBitmap
{
    DisplayBitmap(unsigned int w, unsigned int h, const unsigned char* bm) 
    {
        width = w;
        height = h;
        bitmap = bm;
    }
    unsigned int width;
    unsigned int height;
    const unsigned char* bitmap;
};


#endif