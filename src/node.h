#pragma once

#include <cstddef>
#include <raylib.h>
#include <raymath.h>
#include <vector>

#include "./declaration.h"
#include "./util.h"


class CNode 
{

private:
    
    size_t                  mid;
    Vector2                 position = { 0.0, 0.0 };
    Vector2                 size = { NODE_W, SIGNAL_RADIUS*2};
    bool                    selected = false;
    std::vector<size_t>     signals;


    void update_size();

public:
   
    size_t nb_input = 0;
    size_t nb_output = 0;


    CNode(Vector2 pos);

    void add_signal(size_t sid); 
    void set_selected(bool s);
    void set_position(Vector2 offset);

    std::vector<size_t>& get_signals();

    bool is_selected();

    Vector2& get_position();
    Vector2& get_size();
    Rectangle get_rectangle(); 

};





