#ifndef HID_H
#define HID_H

#include <stdint.h>
#include "emp_type.h"

void hid_init();
void hid_send_controls(INT8S x, INT8S y, INT8S z, INT8U buttonLSB, INT8U buttonMSB);

#endif