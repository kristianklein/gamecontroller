#include <gyro.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "emp_type.h"
#include "rtcs.h"
#include "uart0.h"
#include "file.h"
#include "string.h"

//****************************************DEFINES****************************************
#define MPU_addr            0x68
#define PWR_MGMT_1          0x6B
#define FS_SEL              0x1B
#define ACCEL_XOUT_H        0x3B
#define ACCEL_XOUT_L        0x3C
#define ACCEL_YOUT_H        0x3D
#define ACCEL_YOUT_L        0x3E
#define ACCEL_ZOUT_H        0x3F
#define ACCEL_ZOUT_L        0x40
#define GYRO_XOUT_H         0x43
#define GYRO_XOUT_L         0x44
#define GYRO_YOUT_H         0x45
#define GYRO_YOUT_L         0x46
#define GYRO_ZOUT_H         0x47
#define GYRO_ZOUT_L         0x48


#define ACCELEROMETER_SENSITIVITY 16384
#define GYROSCOPE_SENSITIVITY 32.8
#define ALPHA 0.02

#define M_PI 3.14159265359

#define dt 0.005

//****************************************Variables****************************************
INT16S acc_cali[3];
INT16S gyro_cali[3];
float pitch_cali = 0;
float roll_cali = 0;


//Initialisation of I2C communications
//pins : SCL(PB2) and SDA(PB3)
//Gyro fullscale range: 2000 dps
//I2C data transfer rate: 400 Kbps
//Calibrate gyro and accelerometer offset and store in xxx_cali: calibrate()
void I2C_init(void){
    //enable I2C module 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

    //reset module
    SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);

    //enable GPIO peripheral that contains I2C 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    // Configure the pin muxing for I2C0 functions on port B2 and B3.
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);

    // Select the I2C function for these pins.
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);

    // Enable and initialize the I2C0 master module.  Use the system clock for
    // the I2C0 module.  The last parameter sets the I2C data transfer rate.
    // If false the data rate is set to 100kbps and if true the data rate will
    // be set to 400kbps.
    I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), true);



    //clear I2C FIFOs
    HWREG(I2C0_BASE + I2C_O_FIFOCTL) = 80008000;

    //wake up MPU6050
    I2C_send(MPU_addr, 2, PWR_MGMT_1, 0x00);

    //Set gyro full scale to +-1000 dps
    I2C_send(MPU_addr, 2, FS_SEL, 0b00010000);

    //Calibrate offset and save in xxx_cali arrays
    calibrate(acc_cali, gyro_cali, &pitch_cali, &roll_cali);

}

//sends an I2C command to the specified slave
void I2C_send(INT8U slave_addr, INT8U num_of_args, ...)
{
    // Tell the master module what address it will place on the bus when
    // communicating with the slave.

    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);

    //stores list of variable number of arguments
    va_list vargs;

    //specifies the va_list to "open" and the last fixed argument
    //so vargs knows where to start looking
    va_start(vargs, num_of_args);

    //put data to be sent into FIFO
    I2CMasterDataPut(I2C0_BASE, va_arg(vargs, uint32_t));

    //if there is only one argument, we only need to use the
    //single send I2C function
    if(num_of_args == 1)
    {
        //Initiate send of data from the MCU
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);

        // Wait until MCU is done transferring.
        while(I2CMasterBusy(I2C0_BASE));

        //"close" variable argument list
        va_end(vargs);
    }

    //otherwise, we start transmission of multiple bytes on the
    //I2C bus
    else
    {
        //Initiate send of data from the MCU
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);

        // Wait until MCU is done transferring.
        while(I2CMasterBusy(I2C0_BASE));

        //send num_of_args-2 pieces of data, using the
        //BURST_SEND_CONT command of the I2C module
        for(uint8_t i = 1; i < (num_of_args - 1); i++)
        {
            //put next piece of data into I2C FIFO
            I2CMasterDataPut(I2C0_BASE, va_arg(vargs, uint32_t));
            //send next data that was just placed into FIFO
            I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);

            // Wait until MCU is done transferring.
            while(I2CMasterBusy(I2C0_BASE));
        }

        //put last piece of data into I2C FIFO
        I2CMasterDataPut(I2C0_BASE, va_arg(vargs, uint32_t));
        //send next data that was just placed into FIFO
        I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
        // Wait until MCU is done transferring.
        while(I2CMasterBusy(I2C0_BASE));

        //"close" variable args list
        va_end(vargs);
    }
}


//read specified register on slave device
INT16S I2C_receive(INT8U slave_addr, INT8U reg)
{
    //specify that we are writing (a register address) to the
    //slave device

    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, false);

    //specify register to be read
    I2CMasterDataPut(I2C0_BASE, reg);

    //Check control bit and send register adress byte to slave device.
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_SEND_START);

    //wait for MCU to finish transaction
    while(I2CMasterBusy(I2C0_BASE));

    //specify that we are going to read from slave device
    I2CMasterSlaveAddrSet(I2C0_BASE, slave_addr, true);

    //Check control bit and read from the register we
    //specified
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);

    //wait for MCU to finish transaction
    while(I2CMasterBusy(I2C0_BASE));

    //return data pulled from the specified register
    return I2CMasterDataGet(I2C0_BASE);
}


//Function to read 3 accelerometer values in 1 array and 3 gyroscope values in another (16-bit signed)
void data_read(INT16S accData[3], INT16S gyrData[3]) {
    //recieve highbyte mulitply with 1024 and add lowbyte
  accData[0] = I2C_receive(MPU_addr, ACCEL_XOUT_H) << 8 | I2C_receive(MPU_addr, ACCEL_XOUT_L); // 0x3D (ACCEL_XOUT_H) & 0x3E (ACCEL_XOUT_L)
  accData[1] = I2C_receive(MPU_addr, ACCEL_YOUT_H) << 8 | I2C_receive(MPU_addr, ACCEL_YOUT_L); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  accData[2] = I2C_receive(MPU_addr, ACCEL_ZOUT_H) << 8 | I2C_receive(MPU_addr, ACCEL_ZOUT_L); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  gyrData[0] = I2C_receive(MPU_addr, GYRO_XOUT_H) << 8 | I2C_receive(MPU_addr, GYRO_XOUT_L); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  gyrData[1] = I2C_receive(MPU_addr, GYRO_YOUT_H) << 8 | I2C_receive(MPU_addr, GYRO_YOUT_L); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  gyrData[2] = I2C_receive(MPU_addr, GYRO_ZOUT_H) << 8 | I2C_receive(MPU_addr, GYRO_ZOUT_L); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
}

void complementary_filter(INT16S accData[3], INT16S gyrData[3], float *pitch, float *roll)
{
  float pitchAcc, rollAcc;

  // Integrate the gyroscope data -> int(angularSpeed) = angle
  *pitch += ((float)gyrData[0] / GYROSCOPE_SENSITIVITY) * dt; // Angle around the X-axis
  *roll -= ((float)gyrData[1] / GYROSCOPE_SENSITIVITY) * dt;    // Angle around the Y-axis

  // Turning around the X axis results in a vector on the Y-axis
    pitchAcc = atan2f((float)accData[1], (float)accData[2]) * 180 / M_PI;
    *pitch = *pitch * (1-ALPHA) + pitchAcc * ALPHA;
    // Turning around the Y axis results in a vector on the X-axis
    rollAcc = atan2f((float)accData[0], (float)accData[2]) * 180 / M_PI;
    *roll = *roll * (1-ALPHA) + rollAcc * ALPHA;

}

//measures offset and saves it in acc_cali, gyr_cali, pitch_cali and roll_cali
void calibrate(INT16S acc_cali[3], INT16S gyro_cali[3], float* pitch_cali, float* roll_cali) {
  float pitch_store;
  float roll_store;

  //Measure
  for (int i = 0; i < 1800; i++) {
    data_read( acc_cali, gyro_cali);
    complementary_filter(acc_cali, gyro_cali, pitch_cali, roll_cali);
    if (i >= 900) {
      pitch_store = pitch_store + *pitch_cali;
      roll_store = roll_store +  *roll_cali;
    }
  }

  *pitch_cali = pitch_store / 900;
  *roll_cali = roll_store / 900;
}

//Limits angles to +-45 degress and maps data to +-126 (8bit unsigned)
INT8S map_data(float angleF) {
    INT8S angleI;

    //limit angle to 45 degrees
    if(angleF < -45)
        angleF = -45;
    else if(angleF > 45)
        angleF = 45;

    //map from +-45 to +-126
    angleF *= 2.82;
    return angleI = (INT8S)angleF;
}

//return roll_cali
float get_roll_cali(){
    return roll_cali;
}
