
#pragma once

#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

#include "drawable.h"
#include "utils.h"

class Register {

private:

    std::vector<Circle> circles;
    std::vector<Rect>   rects;
    std::vector<Text>   texts;
    
public:

    void register_node(size_t id, std::string name, int nb_input, int nb_output, Vector2 start);
    
    std::vector<Circle*> get_circles_by_id(size_t id);
    Rect* get_rect_by_id(size_t id);
    Text* get_text_by_id(size_t id);

    std::vector<Circle>& get_circles(); 
    std::vector<Rect>& get_rects();
    std::vector<Text>& get_texts();

    std::vector<Rectangle> get_nodes_position(size_t skip);
};
