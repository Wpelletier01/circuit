

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <raylib.h>
#include <vector>


const int WIDTH = 1280;
const int HEIGHT = 720;


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
    size_t mid;
    Vector2 position = { 0.0, 0.0 };
    std::vector<size_t> signals;
    
public:
    
    CNode(Vector2 pos)
    {
        position = pos;
    }

    void add_signal(size_t sid) { signals.push_back(sid); }


    std::vector<size_t>& get_signals() { return signals; }

    
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


// ==== Helper function






// ==== Draw 


void render_node(size_t nid, Register& reg)
{
    // gather content
    CNode* node = reg.get_node(nid);
    std::vector<Signal*> signals = reg.get_signals(node->get_signals());
    
    std::vector<Signal*> input;
    std::vector<Signal*> output;

    std::copy_if(signals.begin(),signals.end(), std::back_inserter(input) ,[](Signal* s) { return s->t == IoType::Input; });
    std::copy_if(signals.begin(),signals.end(),std::back_inserter(output),[](Signal* s) { return s->t == IoType::Output; });

    
    std::cout << "Node with id: " << nid << " have:\n";
    std::cout << "  output: " << output.size() << "\n";
    std::cout << "  input: " << input.size() << "\n";


}


// ====


int main(void)
{
   
    CModule mod;
    Register reg;
    
    mod.add_node(reg);
    

    for (auto nid : mod.get_nodes()) {
        render_node(nid, reg);
    }
   
    // InitWindow(WIDTH, HEIGHT, "Circuit");

   // while (!WindowShouldClose()) {

   //     
   //     BeginDrawing();
   //         
   //         ClearBackground(GRAY);

   //     EndDrawing();

   // }

   // CloseWindow();

    return 0;
}
