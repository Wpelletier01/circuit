#include "map.h"
#include "primitive.h"




SignalID Map::add_lgate_signal(IoType t)
{
    signals.push_back({sctn,map_id,t,IoLevel::Gate,false,0});
    sctn++;
    return sctn - 1;
}

void Map::update_lgate(size_t index)
{

    
   
    switch (lgates[index].t) {
        
        case LGateType::AND:
            signals[lgates[index].output].activate = signals[lgates[index].xinput].activate & signals[lgates[index].yinput].activate;    
            break;

        case LGateType::OR:
            signals[lgates[index].output].activate = signals[lgates[index].xinput].activate | signals[lgates[index].yinput].activate;    
            break;

        case LGateType::XOR:
            signals[lgates[index].output].activate = signals[lgates[index].xinput].activate ^ signals[lgates[index].yinput].activate;    
            break;

        case LGateType::NOT:
            signals[lgates[index].output].activate = signals[lgates[index].xinput].activate ^ 1;    
            break;

        case LGateType::NAND:
            
            if (signals[lgates[index].xinput].activate == 1 && signals[lgates[index].yinput].activate == 1) {
                signals[lgates[index].output].activate = 0; 
            } else {
                signals[lgates[index].output].activate = 1;
            }

            break;

        case LGateType::NOR:
            if (signals[lgates[index].xinput].activate == 0 && signals[lgates[index].yinput].activate == 0) {
                signals[lgates[index].output].activate = 1; 
            } else {
                signals[lgates[index].output].activate = 0;
            }


            break;
            
        case LGateType::XNOR:
            
            if (signals[lgates[index].xinput].activate == signals[lgates[index].yinput].activate) {
                signals[lgates[index].output].activate = 1; 
            } else {
                signals[lgates[index].output].activate = 0;
            }

            break;

    }

}

void Map::add_maps_signal(IoType t)
{
    signals.push_back({sctn,map_id,t,IoLevel::Map,false,0});
    sctn++;
}


void Map::add_lgate(LGateType t)
{

    SignalID xinput = add_lgate_signal(IoType::Input);
    SignalID yinput = 0;

    if ( t != LGateType::NOT) {
        yinput = add_lgate_signal(IoType::Input);
    }
    
    SignalID output = add_lgate_signal(IoType::Output);

    lgates.push_back({t,xinput,yinput,output});
    
}


void Map::update()
{

}

std::vector<LGate>& Map::get_lgates() { return lgates; }

Signal* Map::get_signal(SignalID sid) { return &signals[sid-1]; }   



