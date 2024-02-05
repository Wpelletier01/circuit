#pragma once


#include <cstddef>
#include <vector>

#include "../core/primitive.h"
#include "register.h"

class CModule 
{

private:
    size_t mid;
    std::vector<size_t> nodes;

public:
    

    void add_node(Register& reg)
    {
        
        // tmp 

        size_t node = reg.register_node({400.f, 400.f });
            
        reg.add_signal_to_node(node, mid, IoType::Input, IoLevel::Node);
        reg.add_signal_to_node(node, mid, IoType::Input, IoLevel::Node);
        reg.add_signal_to_node(node, mid, IoType::Output, IoLevel::Node);

        // ====
        
        nodes.push_back(node);

    }

    std::vector<size_t>& get_nodes() { return nodes; }


};

