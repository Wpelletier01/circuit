
#include "register.h"



size_t Register::register_signal(IoType t, size_t mid, IoLevel lvl, Signal* connect = nullptr) 
{
    signals.push_back(Signal { signals.size(), mid, t, lvl, false, connect });
    return signals.size() - 1;
}

size_t Register::register_node(Vector2 start_pos)
{
    nodes.push_back(CNode(start_pos));    
    return nodes.size() - 1;
}

size_t Register::register_wire(Vector2 pa, Vector2 pb)
{
    wires.push_back({ pa, pb});
    return wires.size() - 1;
}


void Register::add_signal_to_node(size_t nid, size_t mid, IoType t, IoLevel lvl)
{
    
    if ( t == IoType::Input) nodes[nid].nb_input++;
    else nodes[nid].nb_output++;

    size_t sid = register_signal(t, mid,lvl);
    nodes[nid].add_signal(sid); 
    
}   

CNode* Register::get_node(size_t nid) { return &nodes[nid]; }


std::vector<Signal*> Register::get_signals(std::vector<size_t>& sids) 
{ 
    std::vector<Signal*> output;
    
    for ( auto sid : sids ) {
        output.push_back(&signals[sid]);
    }

    return output; 

}
    



