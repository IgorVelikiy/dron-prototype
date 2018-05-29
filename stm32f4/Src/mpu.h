#ifndef MPU_H
#define MPU_H
#include "stm32f4xx.h"
//украдено:
#define ACCEL_XOUT_H       0x3B   // R  
#define ACCEL_XOUT_L       0x3C   // R  
#define ACCEL_YOUT_H       0x3D   // R  
#define ACCEL_YOUT_L       0x3E   // R  
#define ACCEL_ZOUT_H       0x3F   // R  
#define ACCEL_ZOUT_L       0x40   // R

#define TEMP_OUT_H         0x41   // R  
#define TEMP_OUT_L         0x42   // R

#define GYRO_XOUT_H        0x43   // R  
#define GYRO_XOUT_L        0x44   // R  
#define GYRO_YOUT_H        0x45   // R  
#define GYRO_YOUT_L        0x46   // R  
#define GYRO_ZOUT_H        0x47   // R  
#define GYRO_ZOUT_L        0x48   // R

#define CMPS_XOUT_L        0x4A   // R
#define CMPS_XOUT_H        0x4B   // R
#define CMPS_YOUT_L        0x4C   // R
#define CMPS_YOUT_H        0x4D   // R
#define CMPS_ZOUT_L        0x4E   // R
#define CMPS_ZOUT_H        0x4F   // R
/////////////
typedef struct
{
	float x,y,z;
} Coords;

typedef struct
{
	Coords accelerometer, gyroscope, compass;
} MEMS;

void i2cInit(void);
void i2cStart(void);
void i2cStop(void);
uint8_t i2cRead(uint8_t registerAddress);
void i2cWrite(uint8_t registerAddress, uint8_t data);
void MagnetInit(void);
int8_t initMPU(void);
void readMPU(MEMS* mpu);

#endif
