

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <raylib.h>
#include <raymath.h>

// Declaration ===

const int       WIDTH = 1280;
const int       HEIGHT = 720;

const Color     NODE_COLOR = { 75, 94, 80, 255};
const Vector2   NODE_SIZE = { 50.f, 10.f };

const Color     OFF_COLOR = BLACK;

const float     WIRE_THICKNESS = 5.f;

const float     LINK_PADDING = 8.f;
const float     LINK_SIZE = 16.f;
const Color     LINK_COLOR = ORANGE;

// UI
const Color     BACKGROUND = { 64, 64, 64, 255};

const float     MAP_IO_BG_SIZE = LINK_SIZE + (LINK_PADDING*2);

// DATA TYPE ===

enum LinkType 
{
    MAP,
    NODE_IO,
    NODE, 
    IN_DRAW
};

enum NodeType 
{
    LOGIC_GATE,
    LOGIC_CIRCUIT,
    MODULE,
    
};

enum LogicGate
{
    AND,
    OR,
    XOR,
    NOT,
    NAND,
    NOR,
    XNOR
};




// === Utility

std::string get_logic_gate_str(LogicGate lg)
{
    std::string output;

    switch (lg) {

        case LogicGate::AND:
            output = "AND";
            break;
        case LogicGate::OR:
            output = "OR";
            break;
        case LogicGate::XOR:
            output = "XOR";
            break;
        case LogicGate::NOT:
            output = "NOT";
            break;
        case LogicGate::NAND:
            output = "NAND";
            break;
        case LogicGate::NOR:
            output = "NOR";
            break;
        case LogicGate::XNOR:
            output = "XNOR";
            break;
    }

    return output;

}

float get_link_area_height(int nb_signals)
{

    float signal_part = nb_signals * LINK_SIZE;

    float padding_part = 0.f;

    // When only one, no padding space to calculate
    if (nb_signals > 1) {
        padding_part = (nb_signals - 1) * LINK_PADDING;
    }

    return signal_part + padding_part;
}

Vector2 get_rect_center(Rectangle rect) { return { (rect.width / 2) + rect.x, (rect.height / 2) + rect.y};}

// ===



struct Link 
{
    unsigned int        id;
    unsigned int        node_id = 0;
    LinkType            type;
     
    Rectangle           rect = {0.f, 0.f, LINK_SIZE, LINK_SIZE};
    Vector2             offset = {0.f, 0.f};

    std::vector<Link*>  parents;
    std::vector<Link*>  children;

    bool                selected = false;
    bool                render = false;

    
    void set_position(Vector2 pos)
    {
        rect.x = pos.x;
        rect.y = pos.y;
    }

    void set_size(float w, float h)
    {
        rect.width = w;
        rect.height = h;
    }
    
    void mov() 
    {
        rect.x += offset.x;
        rect.y += offset.y;
    }
    
    void reset_offset() { offset = {0.f, 0.f}; }

    Vector2 get_position() { return { rect.x, rect.y}; }

    void add_child(Link *link)
    {
        children.push_back(link);
        //update_size();
    }

    void add_parent(Link *link)
    {   
        parents.push_back(link);
        //update_size();
    }
    
    void update_size() 
    {
        if (type == LinkType::NODE) {
            size_t bigger = std::max(children.size(),parents.size());
            rect.height = get_link_area_height(bigger);
            rect.x = parents[0]->rect.x;
        }
    }

    bool is_not_connected()
    {
        if (type == LinkType::NODE_IO) {

            return !children.empty() && !parents.empty();
        }
        
        // the rest is considered connected for now
        return true;
    }

    bool has_a_wire_draw()
    {
        for (auto child : children) {
            if ( child->type == LinkType::IN_DRAW) return true;
        }
        
        for (auto parent : parents) {
            if ( parent->type == LinkType::IN_DRAW) return true;
        }

        return false;
    }

    void add_drawing_link(Link *link) { 

        if (children.empty()) children.push_back(link);
        else parents.push_back(link);
    }

    void update_drawing_link_position(Vector2 delta) {
        
        for ( auto child : children) {
            if (child->type == LinkType::IN_DRAW) {
                child->offset = delta;
                break;
            }
        } 

        for ( auto parent : parents) {
            if (parent->type == LinkType::IN_DRAW) {
                parent->offset = delta;
                break;
            }
        }
    }

    void remove_drawing_link()
    {
        for (size_t i = 0; i < children.size(); i++) {
            if (children[i]->type == LinkType::IN_DRAW) {
                free(children[i]);
                children.erase(children.begin() + i);
            }
        }
    
        for (size_t i = 0; i < parents.size(); i++) {
            if (parents[i]->type == LinkType::IN_DRAW) {
                free(parents[i]);
                parents.erase(parents.begin() + i);
            }
        }
    
    }

};



struct MapIO
{
     
    std::vector<Link*>  links;
    Rectangle           background;

    
    MapIO(Vector2 position)
    {
        background = { position.x, position.y, MAP_IO_BG_SIZE, HEIGHT};
    
    }

    void update_io_position()
    {

        float height = (HEIGHT / 2.f) - (get_link_area_height(links.size()) / 2.f);

        for ( auto link : links) {
            
            link->rect.y = height;

            height += LINK_PADDING + LINK_SIZE;

        }
        
    }

    void add_io(Link *io)
    {
        
        io->type = LinkType::MAP;

        io->rect.x = background.x + LINK_PADDING;

        links.push_back(io);

        update_io_position();
    }

    void render()
    {
        DrawRectangleRec(background, BACKGROUND); 
    }

    void draw_default_link()
    {  
        DrawRectangleV({background.x + LINK_PADDING, (HEIGHT/2.f) - (LINK_SIZE/2.f)},{LINK_SIZE,LINK_SIZE},LINK_COLOR);
    }

};





// ===

class Register 
{

private:
    
    unsigned int lid = 1;
    unsigned int nid = 1;

public:


    Link* new_link()
    {
        
        Link *link = new Link;

        lid++;

        link->id = lid - 1;

        return link;
    }


};


// Updating === 

void update_position(Link *link)
{

    link->mov();
    link->reset_offset();
    
    for (auto child : link->children) update_position(child);
    for (auto parent : link->parents) {
        if (parent->parents.empty()) {
            parent->mov();
            parent->reset_offset();
        } 
    }
    
}

void collision(Link *link) {
    
    if (link->type == LinkType::NODE) {
        
        // check if node on the I/O platform
        if (link->rect.x + link->offset.x < MAP_IO_BG_SIZE || link->rect.x + link->offset.x + link->rect.width > WIDTH - MAP_IO_BG_SIZE) {        
            link->offset.x = 0.f;   
            
            for (auto parent : link->parents) parent->offset.x = 0.f;
            for (auto child : link->children) child->offset.x = 0.f;

        }

    }

    for (auto child : link->children) collision(child);
   

}

void move_node(Link *link, Vector2 delta)
{
    link->offset = delta;

    for (auto parent : link->parents) parent->offset = delta;
    for (auto child : link->children) child->offset = delta;
    

}

void mouse_input(Link *link)
{
    
    if (link->type == LinkType::NODE) {
        
        Vector2 mpos = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mpos,link->rect)) link->selected = true;
        else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && link->selected) move_node(link,GetMouseDelta());
        else link->selected = false;

    } else if (link->type == LinkType::NODE_IO && link->is_not_connected()) {
        
        Vector2 mpos = GetMousePosition();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mpos,link->rect)) link->selected = true;
        else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && link->selected) {
           
            if(!link->has_a_wire_draw()) {
                
                Link *draw = new Link;
                
                draw->type = LinkType::IN_DRAW;
                draw->set_position(GetMousePosition());
                link->add_drawing_link(draw);
            } else link->update_drawing_link_position(GetMouseDelta());
            

        } else {
            link->selected = false;
            link->remove_drawing_link();
        }

    }

    for (auto child : link->children) mouse_input(child);

}

void update(struct MapIO& minput, struct MapIO& mout)
{
    
    for ( auto input : minput.links) mouse_input(input);
    
    // check for collision 
    for ( auto input : minput.links) collision(input);

    // apply offset
    for ( auto input : minput.links) update_position(input);

}

// Rendering ===



void render_link(Link* link)
{
    
     

    if (link->type == LinkType::MAP) {
        
        for (auto child : link->children) {

            DrawLineBezier(get_rect_center(link->rect),get_rect_center(child->rect),WIRE_THICKNESS,OFF_COLOR);
            render_link(child);
        }

        DrawRectangleRec(link->rect,OFF_COLOR);


    } else if (link->type == LinkType::NODE) {
        
        size_t bigger = std::max(link->children.size(),link->parents.size());

        
        DrawRectangleRec(link->rect,NODE_COLOR);

        // Render Node Link after the background block
        for (auto input : link->parents) {
            if (input->type == LinkType::NODE_IO) DrawRectangleRec(input->rect,OFF_COLOR);
        }

        for (auto output : link->children) {
            if (output->type == LinkType::NODE_IO) DrawRectangleRec(output->rect, OFF_COLOR);
            
            render_link(output);
        }

        
    } else {
        for (auto child : link->children) {
            if (child->type != LinkType::NODE) DrawLineBezier(get_rect_center(link->rect),get_rect_center(child->rect),WIRE_THICKNESS,OFF_COLOR);

            render_link(child);
        }
    }



}


void render_ui(struct MapIO minput, struct MapIO moutput)
{
    minput.render();
    moutput.render();

}

void render(struct MapIO minput, struct MapIO moutput)
{
    render_ui(minput, moutput);
    
    if (moutput.links.size() < 1) moutput.draw_default_link();

    for (auto input : minput.links)  {
        render_link(input);
    }



}



// ===

int main(void)
{
    
    class Register reg;
     

    struct MapIO minput({0.f, 0.f});
    struct MapIO moutput({WIDTH - MAP_IO_BG_SIZE, 0.f});

    minput.add_io(reg.new_link());
    moutput.add_io(reg.new_link());

    // test
    Link *node_input = reg.new_link();
    Link *node = reg.new_link();
    Link *node_output = reg.new_link();
    Link *empty_link = reg.new_link();

    minput.links[0]->add_child(node_input);
    moutput.links[0]->add_parent(node_output);
    
    node_input->type = LinkType::NODE_IO;
    node_input->set_position({ 400.f, 200.f});
    node_input->add_parent(minput.links[0]);
    node_input->add_child(node);

    empty_link->type = LinkType::NODE_IO;
    empty_link->set_position({400.f, 216.f + LINK_PADDING});
    empty_link->add_child(node);

    node->type = LinkType::NODE;
    node->set_position({ 416.f, 200.f});
    node->set_size(3*LINK_SIZE, LINK_SIZE);

    node->add_parent(node_input);
    node->add_parent(empty_link);


    node->add_child(node_output);
    

    node_output->type = LinkType::NODE_IO;
    node_output->set_position({ 432.f, 200.f});
    node_output->add_parent(node);
    node_output->add_child(moutput.links[0]);


    InitWindow(WIDTH, HEIGHT, "Circuit");

    while (!WindowShouldClose()) {

        // update
        update(minput,moutput);
        // ===

        BeginDrawing();

            ClearBackground(GRAY);
            render(minput, moutput); 

        EndDrawing();

    }

    CloseWindow();
    


    return 0;
}


