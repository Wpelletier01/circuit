
#include "renderer.h"


void render(Register &reg)
{
    for ( auto rect : reg.get_rects()) {
        DrawRectangleV(rect.dinfo.position,rect.size,NODE_COLOR);
    }

    for ( auto circle : reg.get_circles()) {
        DrawCircleV(circle.dinfo.position,circle.radius,SIGNAL_COLOR);
        
    }

}
