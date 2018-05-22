/*
 * amplitude.c
 *
 *  Created on: 9. maj 2018
 *      Author: Anders Sørensen
 */
#include <stdint.h>
#include "tasks.h"
#include "taskmodel.h"
#include "global_def.h"
#include "systick.h"
#include "rtcs.h"
#include "amplitude.h"
#include "emp_type.h"
#include "amplitude.h"


INT8U uamplitude(INT8U* array, INT8U size)
{
    INT8U max_value = 0;
    INT8U min_value = 255;

    for (INT8U i = 0; i < size; i++)
    {
        if (array[i] > max_value)
        {
            max_value = array[i];
        }
        if (array[i] < min_value)
        {
            min_value = array[i];
        }
    }
    return((max_value - min_value));
}
