
#include <raylib.h>
#include <raymath.h> 

#include "core/map.h"
#include "core/primitive.h"
#include "graphic/declaration.h"
#include "graphic/renderer.h"
#include "graphic/register.h"
// ==== Update and Logic


void node_mouse(size_t id, Register& reg)
{    
    
    Rect* rect = reg.get_rect_by_id(id);
    std::vector<Circle*> circles = reg.get_circles_by_id(id);
    
    Vector2 mpos = GetMousePosition();
    Rectangle rpos = { rect->dinfo.position.x, rect->dinfo.position.y, rect->size.x, rect->size.y};

    if (CheckCollisionPointRec(mpos, rpos) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        
        rect->dinfo.selected = true;
        
        for ( auto circle : circles) {
            circle->dinfo.selected = true;
        }

    } else if (rect->dinfo.selected && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        
        rect->dinfo.position = Vector2Add(rect->dinfo.position,GetMouseDelta());

        for ( auto circle : circles) {
            circle->dinfo.position = Vector2Add(circle->dinfo.position,GetMouseDelta());
        }

    } else {
        rect->dinfo.selected = false;
        
        for ( auto circle : circles) {
            circle->dinfo.selected = false;
        }
    }

}


void update(class Map& map, Register& reg)
{
    
    std::vector<LGate> lgates = map.get_lgates();

    for (size_t i = 0; i < lgates.size(); i++) {
        node_mouse(i, reg);
    }


}



// ===

int main(void)
{
 
    class Register reg;
    class Map map;

    map.add_lgate(LGateType::OR);
    
    reg.register_node(0, 2, 1, {300.0,300.0});
 

    InitWindow(WIDTH, HEIGHT, "Circuit");

    while (!WindowShouldClose()) {
       
        update(map, reg);

        
        BeginDrawing();
            
            ClearBackground(GRAY);
            
            render(reg);

        EndDrawing();

    }

    CloseWindow();

    return 0;
}
