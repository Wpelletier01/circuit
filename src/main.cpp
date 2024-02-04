

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <raylib.h>
#include <raymath.h>
#include <vector>



// ==== DECLARATION

const int WIDTH = 1280;
const int HEIGHT = 720;

// TMP 
const float NODE_W = 50.f;
const float SIGNAL_RADIUS = 5.f; 

const Color NODE_COLOR = { 75, 94, 80, 255};
const Color SIGNAL_COLOR = { 20, 20, 20, 255 };

const float SIGNAL_PADDING = 5.f;

// 

// ==== Helper function
float get_signal_area(int nb_signals) 
{ 
    
    float signal_part = nb_signals * (SIGNAL_RADIUS*2);
    
    float padding_part = 0;

    // When only one, no padding space to calculate
    if (nb_signals > 1) { 
        padding_part = (nb_signals - 1) * SIGNAL_PADDING;
    }

    return signal_part + padding_part;
}


// ====

enum IoType
{
    Input,
    Output
};

enum IoLevel
{
    Module,
    Node
};

struct Signal 
{
    size_t  id;
    size_t  mid;
    IoType  t;
    IoLevel lvl;
    Signal* connect;
};




class CNode 
{

private:
    
    size_t                  mid;
    Vector2                 position = { 0.0, 0.0 };
    Vector2                 size = { NODE_W, SIGNAL_RADIUS*2};
    bool                    selected = false;
    std::vector<size_t>     signals;


    void update_size()
    {
        size_t bigger = std::max(nb_output,nb_input);
        
        size.y = get_signal_area(bigger);

    }

public:
   
    size_t nb_input = 0;
    size_t nb_output = 0;



    CNode(Vector2 pos)
    {
        position = pos;
    }

    void add_signal(size_t sid) 
    { 
        signals.push_back(sid); 
        update_size();
    }
    
    void set_selected(bool s) { selected = s; }
    void set_position(Vector2 offset) { position = Vector2Add(position, offset); }

    std::vector<size_t>& get_signals() { return signals; }

    bool is_selected() { return selected; }

    Vector2& get_position() { return position; }
    Vector2& get_size() { return size; }
    Rectangle get_rectangle() { return { position.x, position.y, size.x, size.y}; }

};


class Register
{

private:
    std::vector<CNode> nodes;
    std::vector<Signal> signals;
 
public:
    

    size_t register_signal(IoType t, size_t mid, IoLevel lvl, Signal* connect = nullptr) 
    {
        signals.push_back(Signal { signals.size(), mid, t, lvl, connect });
        return signals.size() - 1;
    }
    

    size_t register_node(Vector2 start_pos)
    {
        nodes.push_back(CNode(start_pos));    
        return nodes.size() - 1;
    }
    

    void add_signal_to_node(size_t nid, size_t mid, IoType t, IoLevel lvl)
    {
        
        if ( t == IoType::Input) nodes[nid].nb_input++;
        else nodes[nid].nb_output++;

        size_t sid = register_signal(t, mid,lvl);
        nodes[nid].add_signal(sid); 
        
    }   

    CNode* get_node(size_t nid) { return &nodes[nid]; }
    std::vector<Signal*> get_signals(std::vector<size_t>& sids) 
    { 
        std::vector<Signal*> output;
        
        for ( auto sid : sids ) {
            output.push_back(&signals[sid]);
        }

        return output; 

    }
    


};


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



// ==== Draw 


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


// ==== Update and Logic


void mouse_input(size_t nid, Register& reg)
{
    
    CNode* node = reg.get_node(nid);
    

    // Check if touch node 
    if (CheckCollisionPointRec(GetMousePosition(), node->get_rectangle()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        node->set_selected(true);
        std::cout << "you select node id: " << nid << "\n";

    } else if (node->is_selected() && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        node->set_position(GetMouseDelta());
        std::cout << "you move node id: " << nid << "\n";
    } else {
        node->set_selected(false);
    }



}


void update(CModule& mod, Register& reg)
{
    
    for ( auto nid : mod.get_nodes()) {

        mouse_input(nid, reg);

    }

}



// ====


int main(void)
{
   
    CModule mod;
    Register reg;
    
    mod.add_node(reg);
 
    InitWindow(WIDTH, HEIGHT, "Circuit");

    while (!WindowShouldClose()) {
       

        update(mod,reg);

        
        BeginDrawing();
           
            ClearBackground(GRAY);
            for (auto nid : mod.get_nodes()) {
                render_node(nid, reg);
            }

        EndDrawing();

    }

    CloseWindow();

    return 0;
}
