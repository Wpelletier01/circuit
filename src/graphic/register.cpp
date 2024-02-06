#include "register.h"



void Register::register_node(size_t id, int nb_input, int nb_output, Vector2 start)
{  
    // determine height of the node
    int bigger = std::max(nb_input,nb_output);
    Vector2 size = { NODE_W, get_signal_area(bigger)};

    float posy = ((size.y / 2.f) - (get_signal_area(nb_input) / 2.f)) + start.y + SIGNAL_RADIUS;

    for (int i = 0; i < nb_input; i++) {
        circles.push_back({{id,{start.x+SIGNAL_RADIUS,posy},false},SIGNAL_RADIUS});
        posy += (SIGNAL_RADIUS*2) + SIGNAL_PADDING;
    }
    
    posy = ((size.y / 2.f) - (get_signal_area(nb_output) / 2.f)) + start.y + SIGNAL_RADIUS;
    
    for (int i = 0; i < nb_output; i++) {
        circles.push_back({{id,{(start.x + size.x) - SIGNAL_RADIUS,posy},false},SIGNAL_RADIUS});
        posy += (SIGNAL_RADIUS*2) + SIGNAL_PADDING;
    }
    
    rects.push_back({{id,start,false},size});

}

std::vector<Circle*> Register::get_circles_by_id(size_t id)
{
    std::vector<Circle*> out;
    
    for ( size_t i = 0; i < circles.size(); i++) {
        if (circles[i].dinfo.parent == id) {
            out.push_back(&circles[i]);
        }
    }

    return out;

}

Rect* Register::get_rect_by_id(size_t id)
{
    for (size_t i = 0; i < rects.size(); i++) {
        if (rects[i].dinfo.parent == id) {
            return &rects[i];
        }
    }

    return nullptr;
}

std::vector<Circle>& Register::get_circles()
{
    return circles;
}

std::vector<Rect>& Register::get_rects()
{
    return rects;
}
