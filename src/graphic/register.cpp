#include "register.h"


void Register::register_node(size_t id, std::string name, int nb_input, int nb_output, Vector2 start)
{
    
    // TODO: try to place the text in the center 

    // determine height of the node
    int bigger = std::max(nb_input,nb_output);
    
    float height = std::max(get_signal_area(bigger),(NODE_TEXT_PADDING_Y*2) + NODE_TEXT_SIZE.y);

    // determine width of the node
    float width  = NODE_TEXT_SIZE.x + NODE_TEXT_PADDING_X + ((2*SIGNAL_RADIUS)*2);

    Vector2 size = { width, height };

    // register circle for io

    float posy = ((size.y / 2.f) - (get_signal_area(nb_input) / 2.f)) + start.y + SIGNAL_RADIUS;

    for (int i = 0; i < nb_input; i++) {
        circles.push_back({{id,{start.x+SIGNAL_RADIUS,posy},OType::NODE,false},SIGNAL_RADIUS});
        posy += (SIGNAL_RADIUS*2) + SIGNAL_PADDING;
    }
    
    posy = ((size.y / 2.f) - (get_signal_area(nb_output) / 2.f)) + start.y + SIGNAL_RADIUS;
    
    for (int i = 0; i < nb_output; i++) {
        circles.push_back({{id,{(start.x + size.x) - SIGNAL_RADIUS,posy},OType::NODE,false},SIGNAL_RADIUS});
        posy += (SIGNAL_RADIUS*2) + SIGNAL_PADDING;
    }

    // register rect  
    rects.push_back({{id,start,OType::NODE,false},size});
    // register text 
    texts.push_back(
        {
            {
                id,
                {(size.x/2) - (NODE_TEXT_SIZE.x/2) + start.x + NODE_TEXT_PADDING_X,start.y + NODE_TEXT_PADDING_Y},
                OType::NODE,
                false
            },
            name,
            NODE_TEXT_SIZE
        }
    );
}

void Register::register_wirring(Circle* c) { wirring = c; }


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

Text* Register::get_text_by_id(size_t id)
{
    for (size_t i = 0; i < texts.size(); i++) {
        if (texts[i].dinfo.parent == id) {
            return &texts[i];
        }
    }

    return nullptr;
}


std::vector<Circle>& Register::get_circles() { return circles; }
std::vector<Rect>& Register::get_rects() { return rects; }
std::vector<Text>& Register::get_texts() { return texts; }
std::vector<Wire>& Register::get_wires() { return wires; }
Circle* Register::get_wirring() { return wirring; }

std::vector<Rectangle> Register::get_nodes_position(size_t skip)
{
    
    std::vector<Rectangle> positions;
     
    for ( auto rect : rects) {
        if (rect.dinfo.otype == OType::NODE && rect.dinfo.parent != skip) { 
            positions.push_back({rect.dinfo.position.x,rect.dinfo.position.y,rect.size.x,rect.size.y});
        }
    }

    return positions;

}
