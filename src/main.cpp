


#include <cstddef>
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
    std::vector<Signal*> inputs;
    std::vector<Signal*> outputs;

public:
    
    void add_input(Signal* sp) { inputs.push_back(sp); }
    void add_output(Signal* sp) { outputs.push_back(sp); }

};


class SignalRegister
{
    
private:

    size_t sigctn = 0;

    std::vector<Signal> signals;
 
public:

    Signal* register_signal(IoType t, size_t mid, IoLevel lvl, Signal* connect = nullptr) 
    {
        signals.push_back(Signal { sigctn, mid, t, lvl, connect });
        sigctn++;
        return &signals.back();
    }
    

    std::vector<Signal*> get_module_signal(size_t mid) 
    { 
        
        std::vector<Signal*> out;

        for (auto sp : signals) {
            
            if (sp.lvl == IoLevel::Module && sp.mid == mid ) {
                out.push_back(&sp);
            }
        }

        return out;

    }

};


class NodeRegister
{

private:
    size_t node_ctn = 0;
    std::vector<CNode> nodes;

};


class Module 
{

private:
    size_t mid;
    std::vector<size_t> nodes;

public:
    

    void add_node(SignalRegister& sreg)
    {

    }


};


// ==== Draw 




int main(void)
{
   


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
