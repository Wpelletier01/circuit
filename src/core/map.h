#pragma once

#include <vector>

#include "primitive.h"


class Map 
{

private:
    
    size_t              map_id = 0;
    SignalID            sctn = 1;
    std::vector<Signal> signals; 
    std::vector<LGate>  lgates;

    SignalID add_lgate_signal();

    void update_lgates();

public:

    // IO of the map itself
    void add_maps_signal(IoType);
    void add_lgate(LGateType t);
   

    void update();

};


