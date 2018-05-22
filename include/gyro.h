/*
 * i2c.h
 *
 *  Created on: 3. maj 2018
 *      Author: jespe
 */

#ifndef GYRO_H_
#define GYRO_H_


#include "emp_type.h"

void I2C_init(void);

void I2C_send(INT8U slave_addr, INT8U num_of_args, ...);

INT16S I2C_receive(INT8U slave_addr, INT8U reg);

void data_read(INT16S accData[3], INT16S gyrData[3]);
void complementary_filter(INT16S accData[3], INT16S gyrData[3], float *pitch, float *roll);
void calibrate(INT16S acc_cali[3], INT16S gyro_cali[3], float* pitch_cali, float* roll_cali);

float get_pitch();

float get_roll();

float get_roll_cali();

float get_pitch_cali();

INT8S map_data(float angleF);



#endif /* GYRO_H_ */
