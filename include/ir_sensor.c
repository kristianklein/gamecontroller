#include "ir_sensor.h"

INT8S ir_calc_x(INT8U right_amp, INT8U left_amp)
{
    FP32 x_ratio;
    INT8S x_coord;

    if (right_amp == 0 || left_amp == 0 || right_amp == left_amp) // No signal or equal amplitude. Set cursor in middle of screen.
    {
        x_coord = 0;
    }
    else if (right_amp > left_amp) // Right sensor value is largest
    {
        x_ratio = (FP32)left_amp / (FP32)right_amp;
        x_coord = (1-x_ratio) * -128;
    }
    else // Left sensor value is largest
    {
        x_ratio = (FP32)right_amp / (FP32)left_amp;
        x_coord = (1-x_ratio) * 127;
    }
    
    return x_coord;
}


INT8S ir_calc_y(INT8U top_amp, INT8U bottom_amp)
{
    FP32 y_ratio;
    INT8S y_coord;

    if (top_amp == 0 || bottom_amp == 0 || top_amp == bottom_amp) // No signal or equal amplitude. Set cursor in middle of screen.
    {
        y_coord = 0;
    }
    else if (top_amp > bottom_amp) // Top sensor value is largest
    {
        y_ratio = (FP32)bottom_amp / (FP32)top_amp;
        y_coord = (1-y_ratio) * -128;
    }
    else // Bottom sensor value is largest
    {
        y_ratio = (FP32)top_amp / (FP32)bottom_amp;
        y_coord = (1-y_ratio) * 127;
    }

    return y_coord;
}
