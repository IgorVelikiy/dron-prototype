#include "mpu.h"
#define PI 3.14159265359f

void i2cInit(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; //PB6=SCL,PB7=SDA
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	GPIOB->MODER |= GPIO_MODER_MODER6_1|GPIO_MODER_MODER7_1; //Alternate function
	GPIOB->OTYPER |= GPIO_OTYPER_OT_6|GPIO_OTYPER_OT_7; // Open Drain
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6|GPIO_OSPEEDER_OSPEEDR7;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR6_0|GPIO_PUPDR_PUPDR7_0;
	GPIOB->AFR[0] |= 0x44000000; // AF4 = I2C
	I2C1->CR2 = 42;
	I2C1->CCR = 52|I2C_CCR_DUTY;
	I2C1->TRISE = 43;
	I2C1->CR1 = I2C_CR1_PE;
}

void i2cStart(void)
{
	I2C1->CR1 |= I2C_CR1_START;
	while (!(I2C1->SR1 & I2C_SR1_SB));
}

void i2cStop(void)
{
	I2C1->CR1 |= I2C_CR1_STOP;
}

uint8_t i2cRead(uint8_t registerAddress)
{
	i2cStart();
	I2C1->DR = 0xD0;
	while (!(I2C1->SR1 & I2C_SR1_ADDR));
	uint16_t temp = I2C1->SR1;
	temp = I2C1->SR2;
	I2C1->DR = registerAddress;
	while (!(I2C1->SR1 & I2C_SR1_TXE));
	i2cStart();
	I2C1->DR = 0xD1;
	while (!(I2C1->SR1 & I2C_SR1_ADDR));
	temp = I2C1->SR1;
	temp = I2C1->SR2;
	while (!(I2C1->SR1 & I2C_SR1_RXNE));
	i2cStop();
	//for (int i = 0; i < 1680; i++);
	return I2C1->DR;
}

void i2cWrite(uint8_t registerAddress, uint8_t data)
{
	i2cStart();
	I2C1->DR = 0xD0;
	while (!(I2C1->SR1 & I2C_SR1_ADDR));
	uint16_t temp = I2C1->SR1;
	temp = I2C1->SR2;
	I2C1->DR = registerAddress;
	while (!(I2C1->SR1 & I2C_SR1_TXE));
	I2C1->DR = data;
	while (!(I2C1->SR1 & I2C_SR1_TXE));
	i2cStop();
}

void MagnetInit(void)
{
	i2cWrite(0x24, 0x40);
	i2cWrite(0x25, 0x8C);
	i2cWrite(0x26, 0x02); 
	i2cWrite(0x27, 0x88);
	i2cWrite(0x28, 0x0C); 
	i2cWrite(0x29, 0x0A); 
	i2cWrite(0x2A, 0x81); 
	i2cWrite(0x64, 0x01); 
	i2cWrite(0x67, 0x03); 
	i2cWrite(0x01, 0x80);
	i2cWrite(0x34, 0x04);
	i2cWrite(0x64, 0x00);
	i2cWrite(0x6A, 0x00);
	i2cWrite(0x64, 0x01);
	i2cWrite(0x6A, 0x20);
	i2cWrite(0x34, 0x13);
}

int8_t initMPU(void)
{
	i2cInit();
	if (i2cRead(0x75) != 0x68)
		return -1;
	//i2cWrite(0x1C,0x10); // +-8g
	//i2cWrite(0x1B,0x10);
	//i2cWrite(0x1A, 6);
	i2cWrite(0x6B, 0); // wakeup mpu
	MagnetInit();
	i2cWrite(0x6B, 0x80);
  i2cWrite(0x6B, 0x01);  
  i2cWrite(0x6C, 0x00); 
  
  
// Configure MPU9150 gyro and accelerometer for bias calculation
  i2cWrite(0x1A, 0x01);      // Set low-pass filter to 188 Hz
  i2cWrite(0x19, 0x00);  // Set sample rate to 1 kHz	return 1;
}

void readMPU(MEMS* mpu)
{
	mpu->accelerometer.x = (int16_t)(((i2cRead(ACCEL_XOUT_H)<<8)|i2cRead(ACCEL_XOUT_L)))/16384.0f;
	mpu->accelerometer.y = (int16_t)(((i2cRead(ACCEL_YOUT_H)<<8)|i2cRead(ACCEL_YOUT_L)))/16384.0f;
	mpu->accelerometer.z = (int16_t)(((i2cRead(ACCEL_ZOUT_H)<<8)|i2cRead(ACCEL_ZOUT_L)))/16384.0f;
	mpu->gyroscope.x = (int16_t)(((i2cRead(GYRO_XOUT_H)<<8)|i2cRead(GYRO_XOUT_L)))/131.0f;
	mpu->gyroscope.y = (int16_t)(((i2cRead(GYRO_YOUT_H)<<8)|i2cRead(GYRO_YOUT_L)))/131.0f;
	mpu->gyroscope.z = (int16_t)(((i2cRead(GYRO_ZOUT_H)<<8)|i2cRead(GYRO_ZOUT_L)))/131.0f;
	mpu->compass.x = (int16_t)(((i2cRead(CMPS_XOUT_H)<<8)|i2cRead(CMPS_XOUT_L)))*10*1229/4096;
	mpu->compass.y = (int16_t)(((i2cRead(CMPS_YOUT_H)<<8)|i2cRead(CMPS_YOUT_L)))*10*1229/4096;
	mpu->compass.z = (int16_t)(((i2cRead(CMPS_ZOUT_H)<<8)|i2cRead(CMPS_ZOUT_L)))*10*1229/4096;
}
