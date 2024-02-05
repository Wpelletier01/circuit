#include "map.h"

void Map::add_maps_signal(IoType t)
{
    signals.push_back({sctn,map_id,t,IoLevel::Map,false,0});
    sctn++;
}


SignalID Map::add_lgate_signal()
{
    
}

void Map::add_lgate(LGateType t)
{


    if ( t != LGateType::NOT) {

    }
    
}
    
