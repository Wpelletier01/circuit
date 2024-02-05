#pragma once

#include <cstddef>
#include <raylib.h>

enum IoType
{
    Input,
    Output
};

enum IoLevel
{
    Module,
    Node
};

struct Signal 
{
    size_t  id;
    size_t  mid;
    IoType  t;
    IoLevel lvl;
    bool    selected;
    Signal* connect;
};

struct Wire 
{
    Vector2 pa;
    Vector2 pb;
};


