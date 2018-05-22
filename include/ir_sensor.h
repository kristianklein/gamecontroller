#ifndef IR_SENSOR_H
#define IR_SENSOR_H

#include <stdint.h>
#include "emp_type.h"

INT8S ir_calc_x(INT8U right_amp, INT8U left_amp);
INT8S ir_calc_y(INT8U top_amp, INT8U bottom_amp);

#endif
