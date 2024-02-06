
#pragma once

#include <raylib.h>
#include <vector>
#include <algorithm>

#include "drawable.h"
#include "utils.h"

class Register {

private:

    std::vector<Circle> circles;
    std::vector<Rect>   rects;
    
public:

    void register_node(size_t id, int nb_input, int nb_output, Vector2 start);
    
    std::vector<Circle*> get_circles_by_id(size_t id);
    Rect* get_rect_by_id(size_t id);

    std::vector<Circle>& get_circles(); 
    std::vector<Rect>& get_rects();
};
