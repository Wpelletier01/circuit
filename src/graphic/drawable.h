
#pragma once


#include <cstddef>
#include <raylib.h>

#include "declaration.h"

struct DrawInfo 
{
    size_t      parent;
    Vector2     position;
    bool        selected;
};


struct Circle 
{
    DrawInfo    dinfo;
    float       radius;
    
};


struct Rect 
{
    DrawInfo    dinfo;
    Vector2     size;

};




