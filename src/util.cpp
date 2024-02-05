
#include "util.h"



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


