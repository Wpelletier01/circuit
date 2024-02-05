#pragma once

#include <vector>
#include <raylib.h>

#include "primitive.h"
#include "node.h"


class Register
{

private:
    std::vector<CNode> nodes;
    std::vector<Signal> signals;
    std::vector<Wire> wires;
 
public:
    

    size_t register_signal(IoType t, size_t mid, IoLevel lvl, Signal* connect); 
    size_t register_node(Vector2 start_pos); 
    size_t register_wire(Vector2 pa, Vector2 pb);

    void add_signal_to_node(size_t nid, size_t mid, IoType t, IoLevel lvl);
    CNode* get_node(size_t nid);
    std::vector<Signal*> get_signals(std::vector<size_t>& sids);


};


