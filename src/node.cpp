
#include "node.h"


CNode::CNode(Vector2 pos)
{
    position = pos;
}


void CNode::update_size()
{
    size_t bigger = std::max(nb_output,nb_input);    
    size.y = get_signal_area(bigger);

}


void CNode::add_signal(size_t sid) 
{ 
    signals.push_back(sid); 
    update_size();
}

void CNode::set_selected(bool s) { selected = s; }
void CNode::set_position(Vector2 offset) { position = Vector2Add(position, offset); }

std::vector<size_t>& CNode::get_signals() { return signals; }

bool CNode::is_selected() { return selected; }

Vector2& CNode::get_position() { return position; }
Vector2& CNode::get_size() { return size; }
Rectangle CNode::get_rectangle() { return { position.x, position.y, size.x, size.y}; }


