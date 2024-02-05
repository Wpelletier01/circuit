
#include "render.h"


void render_node(size_t nid, Register& reg)
{
    // gather content
    CNode* node = reg.get_node(nid);
    std::vector<Signal*> signals = reg.get_signals(node->get_signals());
    
    std::vector<Signal*> inputs;
    std::vector<Signal*> outputs;

    std::copy_if(signals.begin(),signals.end(), std::back_inserter(inputs) ,[](Signal* s) { return s->t == IoType::Input; });
    std::copy_if(signals.begin(),signals.end(),std::back_inserter(outputs),[](Signal* s) { return s->t == IoType::Output; });

    // Draw node body
    Vector2 node_pos = node->get_position();
    Vector2 node_size = node->get_size();
    DrawRectangleV(node_pos,node_size,NODE_COLOR);
    
    // Draw node's input 

    float posy = ((node_size.y / 2.f) - (get_signal_area(inputs.size()) / 2.f)) + node_pos.y + SIGNAL_RADIUS;

    for (const auto input : inputs ) {
        
        DrawCircleV({node_pos.x + SIGNAL_RADIUS, posy},SIGNAL_RADIUS,SIGNAL_COLOR); 
        posy += (SIGNAL_RADIUS*2) + SIGNAL_PADDING;
      
    }
    
    // Draw node output 

    posy = ((node_size.y / 2.f) - (get_signal_area(outputs.size()) / 2.f)) + node_pos.y + SIGNAL_RADIUS;

    for ( const auto output : outputs) {

        DrawCircleV({(node_pos.x + node_size.x) - SIGNAL_RADIUS, posy},SIGNAL_RADIUS,SIGNAL_COLOR); 
        posy += (SIGNAL_RADIUS*2) + SIGNAL_PADDING;
    

    }

    
}


