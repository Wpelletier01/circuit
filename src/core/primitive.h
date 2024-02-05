#pragma once

#include <cstddef>
#include <raylib.h>

using SignalID = size_t;


enum IoType
{
    Input,
    Output
};

enum IoLevel
{
    Map,
    Gate
};

enum LGateType 
{
    AND,
    OR,
    XOR,
    NOT,
    NAND,
    NOR,
    XNOR
};

struct Signal 
{
    SignalID    id;
    size_t      mid;
    IoType      t;
    IoLevel     lvl;
    bool        activate;
    size_t      connect;
};


struct LGate
{
    LGateType t;
    SignalID xinput;
    SignalID yinput;
    SignalID output;
};


