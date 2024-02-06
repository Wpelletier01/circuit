
#include <raylib.h>
#include <raymath.h> 

#include "core/map.h"
#include "core/primitive.h"
#include "graphic/declaration.h"
#include "graphic/register.h"

// ==== Update and Logic



class App 
{
  
private:
    class Register  reg;
    class Map       map;


    void node_mouse_selection(Rect* rect, std::vector<Circle*> circles, Text* text)
    {    
     
        Vector2 mpos = GetMousePosition();
        Rectangle rpos = { rect->dinfo.position.x, rect->dinfo.position.y, rect->size.x, rect->size.y};

        if (CheckCollisionPointRec(mpos, rpos) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            
            rect->dinfo.selected = true;
            
            for ( auto circle : circles) {
                circle->dinfo.selected = true;
            }
            
            text->dinfo.selected = true;

        } else if (rect->dinfo.selected && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            
            Vector2 offset = GetMouseDelta();

            rect->dinfo.position = Vector2Add(rect->dinfo.position,offset);

            for ( auto circle : circles) {
                circle->dinfo.position = Vector2Add(circle->dinfo.position,offset);
            }
            
            text->dinfo.position = Vector2Add(text->dinfo.position,offset);


        } else {
            rect->dinfo.selected = false;
            
            for ( auto circle : circles) {
                circle->dinfo.selected = false;
            }

            text->dinfo.selected = false;
        }

    }
    

    void update_nodes()
    {
        std::vector<LGate> lgates = map.get_lgates();

        for ( auto lgate : lgates ) {

            Rect* rect = reg.get_rect_by_id(lgate.id);
            std::vector<Circle*> circles = reg.get_circles_by_id(lgate.id);
            Text* text = reg.get_text_by_id(lgate.id);

            node_mouse_selection(rect,circles,text);
        }


    }


public:

    
    void update()
    {
    
        update_nodes();

    }


    void render()
    {
        // Render node 
        for ( auto rect : reg.get_rects()) {
            DrawRectangleV(rect.dinfo.position,rect.size,NODE_COLOR);
        }

        for ( auto circle : reg.get_circles()) {
            DrawCircleV(circle.dinfo.position,circle.radius,SIGNAL_COLOR);
        }

        for ( auto text : reg.get_texts()) {
            DrawTextEx(GetFontDefault(),text.data.c_str(),text.dinfo.position,text.size.y,5.f,WHITE);
        }

        // ===

    

    }
    

    void add_classic_logic_gate(LGateType t, Vector2 start)
    {
        
        size_t id = map.add_lgate(t);
        
        switch (t) {
            
            case LGateType::AND:
                reg.register_node(id, "AND", 2, 1, start);
                break;
            case LGateType::OR:
                reg.register_node(id, "OR", 2, 1, start);
                break;
            case LGateType::XOR:
                reg.register_node(id, "XOR", 2, 1, start);
                break;
            case LGateType::NOT:
                reg.register_node(id, "NOT", 1, 1, start);
                break;
            case LGateType::NAND:
                reg.register_node(id, "NAND", 2, 1, start);
                break;
            case LGateType::NOR:
                reg.register_node(id, "NOR", 2, 1, start);
                break;
            case LGateType::XNOR:
                reg.register_node(id, "XNOR", 2, 1, start);
                break;
        }
    
    }

};


// ===

int main(void)
{
 
    App app;

    app.add_classic_logic_gate(LGateType::OR, { 300.f, 300.f}); 
    app.add_classic_logic_gate(LGateType::NOT, { 500.f , 200.f});

    InitWindow(WIDTH, HEIGHT, "Circuit");

    while (!WindowShouldClose()) {
       
        app.update();

        
        BeginDrawing();
            
            ClearBackground(GRAY);
            
            app.render();

        EndDrawing();

    }

    CloseWindow();

    return 0;
}
