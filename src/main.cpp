
#include <raylib.h>
#include "graphic/render.h"



// ==== Update and Logic


void mouse_input(size_t nid, Register& reg)
{
    
    CNode* node = reg.get_node(nid);
    
    // Check if touch node 
    if (CheckCollisionPointRec(GetMousePosition(), node->get_rectangle()) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        node->set_selected(true);
    } else if (node->is_selected() && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        node->set_position(GetMouseDelta());
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



// ===

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
