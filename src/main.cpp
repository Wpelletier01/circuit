

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <raylib.h>

// Declaration ===

const int       WIDTH = 1280;
const int       HEIGHT = 720;

const Color     NODE_COLOR = { 75, 94, 80, 255};
const Vector2   NODE_SIZE = { 50.f, 10.f };

const Color     WIRE_COLOR = BLACK;
const float     WIRE_THICKNESS = 5.f;

const float     LINK_RADIUS =  8.f;
const float     LINK_PADDING = 8.f;
const float     LINK_SIZE = 16.f;
const Color     LINK_COLOR = ORANGE;

// UI
const Color     BACKGROUND = { 64, 64, 64, 255};

const float     MAP_IO_BG_SIZE = (LINK_RADIUS*2) + (LINK_RADIUS*2);

// DATA TYPE ===

enum LinkType 
{
    MAP,
    NODE_IO,
    NODE, 
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

// ===



struct Link 
{
    unsigned int        id;
    unsigned int        node_id = 0;
    LinkType            type;
     
    Rectangle           rect = {0.f, 0.f, LINK_SIZE, LINK_SIZE};
    
    std::vector<Link*>  parents;
    std::vector<Link*>  children;

    bool                selected = false;
    bool                render = false;

    
    void set_position(Vector2 pos)
    {
        rect.x = pos.x;
        rect.y = pos.y;
    }


    Vector2 get_position()
    {
        return { rect.x, rect.y};
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


void update(struct MapIO min, struct MapIO mout)
{
    


}

// Rendering ===



void render_link(Link* link)
{
    if (link->type == LinkType::MAP) {
        
        DrawRectangleRec(link->rect,LINK_COLOR);
        for (auto child : link->children) {
            DrawLineBezier(link->get_position(),child->get_position(),5.f,WHITE);
            render_link(child);
        }


    } else if (link->type == LinkType::NODE) {
        
        size_t bigger = std::max(link->children.size(),link->parents.size());
        Rectangle rect = { 400.f - LINK_RADIUS, 200.f - LINK_RADIUS, 55.f, get_link_area_height(bigger)};
        
        DrawRectangleRec(rect,NODE_COLOR);

        // Render Node Link after the background block
        for (auto input : link->parents) {
            if (input->type == LinkType::NODE_IO) DrawRectangleRec(input->rect,LINK_COLOR);
        }

        for (auto output : link->children) {
            if (output->type == LinkType::NODE_IO) DrawRectangleRec(output->rect, LINK_COLOR);
            
            render_link(output);
        }

        
    } else {
        for (auto child : link->children) {
            if (child->type != LinkType::NODE) DrawLineBezier(link->get_position(),child->get_position(),5.f,WHITE);
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


    minput.links[0]->children.push_back(node_input);
    moutput.links[0]->parents.push_back(node_output);
    
    node_input->type = LinkType::NODE_IO;
    node_input->set_position({ 400.f, 200.f});
    node_input->parents.push_back(minput.links[0]);
    node_input->children.push_back(node);


    node->type = LinkType::NODE;
    node->set_position({ 420.f, 200.f});
    node->parents.push_back(node_input);
    node->children.push_back(node_output);
    

    node_output->type = LinkType::NODE_IO;
    node_output->set_position({ 440.f, 200.f});
    node_output->parents.push_back(node);
    node_output->children.push_back(moutput.links[0]);


    InitWindow(WIDTH, HEIGHT, "Circuit");

    while (!WindowShouldClose()) {

        // update
       // update(root1);
        // ===

        BeginDrawing();

            ClearBackground(GRAY);
            render(minput, moutput); 
        //    render_node(root1);

        EndDrawing();

    }

    CloseWindow();
    


    return 0;
}


