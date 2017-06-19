/***************************************************************************//**
 *   @file   ADXL345.c
 *   @brief  Implementation of ADXL345 Driver.
 *   @author DBogdan (dragos.bogdan@analog.com)
********************************************************************************
 * Copyright 2012(c) Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  - Neither the name of Analog Devices, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *  - The use of this software may or may not infringe the patent rights
 *    of one or more patent holders.  This license does not release you
 *    from the requirement that you obtain separate licenses from these
 *    patent holders to use this software.
 *  - Use of the software either in source or binary form, must be run
 *    on or directly connected to an Analog Devices Inc. component.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
********************************************************************************
 *   SVN Revision: 365
*******************************************************************************/

/******************************************************************************/
/* Include Files                                                              */
/******************************************************************************/
#include "ADXL345.h"		// ADXL345 definitions.
#include "i2cdev.h"


static uint8_t devAddr;
static I2C_Dev *I2Cx;
static bool isInit;


/***************************************************************************//**
 * @brief Writes data into a register.
 *
 * @param registerAddress - Address of the register.
 * @param registerValue - Data value to write.
 *
 * @return None.
*******************************************************************************/
#if 0
void ADXL345_SetRegisterValue(unsigned char registerAddress,
							  unsigned char registerValue)
{
	unsigned char writeData[3] = {0, 0, 0};
	writeData[0] = ADXL345_ADDRESS;
	writeData[1] = registerAddress;
	writeData[2] = registerValue;

	I2C_Write(writeData,2);
}
#else
void ADXL345_SetRegisterValue(unsigned char registerAddress,
                unsigned char registerValue)
{
  i2cdevWriteByte(I2Cx, devAddr, registerAddress, registerValue);
}

#endif

/***************************************************************************//**
 * @brief Reads the value of a register.
 *
 * @param registerAddress - Address of the register.
 *
 * @return registerValue - Value of the register.
*******************************************************************************/
#if 0
unsigned char ADXL345_GetRegisterValue(unsigned char registerAddress)
{
	unsigned char readData[2] = {0, 0};
	readData[0] = ADXL345_ADDRESS;
	readData[1] = registerAddress;
	I2C_Read(readData,1);

	return(readData[0]);
}
#else
unsigned char ADXL345_GetRegisterValue(unsigned char registerAddress)
{
  uint8_t byte;

  i2cdevReadByte(I2Cx, devAddr, registerAddress, &byte);

  return byte;
}

#endif


/***************************************************************************//**
 * @brief Initializes the I2C peripheral and checks if the ADXL345 part
 		  is present.
 *
 * @param None.
 *
 * @return status - Result of the initialization procedure.
 					Example: 0x0 - I2C peripheral was not initialized or
								   ADXL345 part is not present.
 				  			 0x1 - I2C peripheral is initialized and ADXL345
							 	   part is present.
*******************************************************************************/
unsigned char ADXL345_Init(void)
{

  if (isInit)
    return true;

	devAddr = 0x53;
	I2Cx = I2C2;

	if (!i2cdevInit(I2C2))
	  return false;

	if(ADXL345_GetRegisterValue(ADXL345_DEVID) != ADXL345_ID)
	  return false;


  ADXL345_SetRegisterValue(0x31,0x2B);  //数据通信格式;设置为自检功能禁用,4线制SPI接口,低电平中断输出,13位全分辨率,输出数据右对齐,16g量程

  ADXL345_SetRegisterValue(0x1E,0x00);  //X轴误差补偿; (15.6mg/LSB)

  ADXL345_SetRegisterValue(0x1F,0x00);  //Y轴误差补偿; (15.6mg/LSB)

  ADXL345_SetRegisterValue(0x20,0x00);  //Z轴误差补偿; (15.6mg/LSB)

  ADXL345_SetRegisterValue(0x21,0x00);  //敲击延时0:禁用; (1.25ms/LSB)

  ADXL345_SetRegisterValue(0x22,0x00);  //检测第一次敲击后的延时0:禁用; (1.25ms/LSB)

  ADXL345_SetRegisterValue(0x23,0x00);  //敲击窗口0:禁用; (1.25ms/LSB)

  ADXL345_SetRegisterValue(0x24,0x01);  //保存检测活动阀值; (62.5mg/LSB)

  ADXL345_SetRegisterValue(0x25,0x01);  //保存检测静止阀值; (62.5mg/LSB)

  ADXL345_SetRegisterValue(0x26,0x2B);  //检测活动时间阀值; (1s/LSB)

  ADXL345_SetRegisterValue(0x27,0x00);  //

  ADXL345_SetRegisterValue(0x28,0x09);  //自由落体检测推荐阀值; (62.5mg/LSB)

  ADXL345_SetRegisterValue(0x29,0xFF);  //自由落体检测时间阀值,设置为最大时间; (5ms/LSB)

  ADXL345_SetRegisterValue(0x2A,0x80);  //禁止敲击检测

  ADXL345_SetRegisterValue(0x2C,0x0B);  //  200hz 输出     0A是100hz输出

  ADXL345_SetRegisterValue(0x2D,0x28);  //开启Link,测量功能;关闭自动休眠,休眠,唤醒功能

  ADXL345_SetRegisterValue(0x2E,0x00);  //所有均关闭

  ADXL345_SetRegisterValue(0x2F,0x00);  //中断功能设定,不使用中断

  ADXL345_SetRegisterValue(0x38,0x9F);  //FIFO模式设定,bypass模式，,31级样本缓冲

  isInit = true;
	return true;
}

/***************************************************************************//**
 * @brief Places the device into standby/measure mode.
 *
 * @param pwrMode - Power mode.
 *					Example: 0x0 - standby mode.
 *							 0x1 - measure mode.
 *
 * @return None.
*******************************************************************************/
void ADXL345_SetPowerMode(unsigned char pwrMode)
{
	unsigned char oldPowerCtl = 0x0;
	unsigned char newPowerCtl = 0x0;
	oldPowerCtl = ADXL345_GetRegisterValue(ADXL345_POWER_CTL);
	newPowerCtl = oldPowerCtl & ~ADXL345_PCTL_MEASURE;
	newPowerCtl = newPowerCtl | (pwrMode * ADXL345_PCTL_MEASURE);
	ADXL345_SetRegisterValue(ADXL345_POWER_CTL, newPowerCtl);
}

/***************************************************************************//**
 * @brief Reads the output data of each axis.
 *
 * @param x - X-axis's output data.
 * @param y - Y-axis's output data.
 * @param z - Z-axis's output data.
 *
 * @return None.
*******************************************************************************/
void ADXL345_GetXyz(unsigned short* x,
					unsigned short* y,
					unsigned short* z)
{
	*x = ADXL345_GetRegisterValue(ADXL345_DATAX1) << 8;
	*x += ADXL345_GetRegisterValue(ADXL345_DATAX0);
	*y = ADXL345_GetRegisterValue(ADXL345_DATAY1) << 8;
	*y += ADXL345_GetRegisterValue(ADXL345_DATAY0);
	*z = ADXL345_GetRegisterValue(ADXL345_DATAZ1) << 8;
	*z += ADXL345_GetRegisterValue(ADXL345_DATAZ0);
}

/***************************************************************************//**
 * @brief Enables/disables the tap detection.
 *
 * @param tapType - Tap type (none, single, double).
 *					Example: 0x0 - disables tap detection.	
 *							 ADXL345_SINGLE_TAP - enables single tap detection.
 *							 ADXL345_DOUBLE_TAP - enables double tap detection.
 * @param tapAxes - Axes which participate in tap detection.
 *					Example: 0x0 - disables axes participation.
 *							 ADXL345_TAP_X_EN - enables x-axis participation.
 *							 ADXL345_TAP_Y_EN - enables y-axis participation.
 *							 ADXL345_TAP_Z_EN - enables z-axis participation.
 * @param tapDur - Tap duration.
 * @param tapLatent - Tap latency.
 * @param tapWindow - Tap window. 
 * @param tapThresh - Tap threshold.
 * @param tapInt - Interrupts pin.
 *				   Example: 0x0 - interrupts on INT1 pin.
 *							ADXL345_SINGLE_TAP - single tap interrupts on
 *												 INT2 pin.
 *							ADXL345_DOUBLE_TAP - double tap interrupts on
 *												 INT2 pin.
 *
 * @return None.
*******************************************************************************/
void ADXL345_SetTapDetection(unsigned char tapType,
							 unsigned char tapAxes,
							 unsigned char tapDur,
							 unsigned char tapLatent,
							 unsigned char tapWindow,
							 unsigned char tapThresh,
							 unsigned char tapInt)
{
	unsigned char oldTapAxes = 0x0;
	unsigned char newTapAxes = 0x0;
	unsigned char oldIntMap = 0x0;
	unsigned char newIntMap = 0x0;
	unsigned char oldIntEnable = 0x0;
	unsigned char newIntEnable = 0x0;
	oldTapAxes = ADXL345_GetRegisterValue(ADXL345_TAP_AXES);
	newTapAxes = oldTapAxes & ~(ADXL345_TAP_X_EN |
								ADXL345_TAP_Y_EN |
								ADXL345_TAP_Z_EN);
	newTapAxes = newTapAxes | tapAxes;
	ADXL345_SetRegisterValue(ADXL345_TAP_AXES, newTapAxes);
	ADXL345_SetRegisterValue(ADXL345_DUR, tapDur);
	ADXL345_SetRegisterValue(ADXL345_LATENT, tapLatent);
	ADXL345_SetRegisterValue(ADXL345_WINDOW, tapWindow);
	ADXL345_SetRegisterValue(ADXL345_THRESH_TAP, tapThresh);
	oldIntMap = ADXL345_GetRegisterValue(ADXL345_INT_MAP);
	newIntMap = oldIntMap & ~(ADXL345_SINGLE_TAP | ADXL345_DOUBLE_TAP);
	newIntMap = newIntMap | tapInt;
	ADXL345_SetRegisterValue(ADXL345_INT_MAP, newIntMap);
	oldIntEnable = ADXL345_GetRegisterValue(ADXL345_INT_ENABLE);
	newIntEnable = oldIntEnable & ~(ADXL345_SINGLE_TAP | ADXL345_DOUBLE_TAP);
	newIntEnable = newIntEnable | tapType;
	ADXL345_SetRegisterValue(ADXL345_INT_ENABLE, newIntEnable);
}

/***************************************************************************//**
 * @brief Enables/disables the activity detection.
 *
 * @param actOnOff - Enables/disables the activity detection.
 *					 Example: 0x0 - disables the activity detection.
 *							  0x1 - enables the activity detection.
 * @param actAxes - Axes which participate in detecting activity.
 *					Example: 0x0 - disables axes participation.
 *							 ADXL345_ACT_X_EN - enables x-axis participation.
 *							 ADXL345_ACT_Y_EN - enables y-axis participation.
 *							 ADXL345_ACT_Z_EN - enables z-axis participation.
 * @param actAcDc - Selects dc-coupled or ac-coupled operation.
 *					Example: 0x0 - dc-coupled operation.
 *							 ADXL345_ACT_ACDC - ac-coupled operation.
 * @param actThresh - Threshold value for detecting activity.
 * @patam actInt - Interrupts pin.
 *				   Example: 0x0 - activity interrupts on INT1 pin.
 *							ADXL345_ACTIVITY - activity interrupts on INT2 pin.
 *
 * @return None.
*******************************************************************************/
void ADXL345_SetActivityDetection(unsigned char actOnOff,
								  unsigned char actAxes,
								  unsigned char actAcDc,
								  unsigned char actThresh,
								  unsigned char actInt)
{
	unsigned char oldActInactCtl = 0x0;
	unsigned char newActInactCtl = 0x0;
	unsigned char oldIntMap = 0x0;
	unsigned char newIntMap = 0x0;
	unsigned char oldIntEnable = 0x0;
	unsigned char newIntEnable = 0x0;
	oldActInactCtl = ADXL345_GetRegisterValue(ADXL345_INT_ENABLE);
	newActInactCtl = oldActInactCtl & ~(ADXL345_ACT_ACDC |
										ADXL345_ACT_X_EN |
										ADXL345_ACT_Y_EN |
										ADXL345_ACT_Z_EN);
	newActInactCtl = newActInactCtl | (actAcDc | actAxes);
	ADXL345_SetRegisterValue(ADXL345_ACT_INACT_CTL, newActInactCtl);
	ADXL345_SetRegisterValue(ADXL345_THRESH_ACT, actThresh);
	oldIntMap = ADXL345_GetRegisterValue(ADXL345_INT_MAP);
	newIntMap = oldIntMap & ~(ADXL345_ACTIVITY);
	newIntMap = newIntMap | actInt;
	ADXL345_SetRegisterValue(ADXL345_INT_MAP, newIntMap);
	oldIntEnable = ADXL345_GetRegisterValue(ADXL345_INT_ENABLE);
	newIntEnable = oldIntEnable & ~(ADXL345_ACTIVITY);
	newIntEnable = newIntEnable | (ADXL345_ACTIVITY * actOnOff);
	ADXL345_SetRegisterValue(ADXL345_INT_ENABLE, newIntEnable);
}

/***************************************************************************//**
 * @brief Enables/disables the inactivity detection.
 *
 * @param inactOnOff - Enables/disables the inactivity detection.
 *					   Example: 0x0 - disables the inactivity detection.
 *							    0x1 - enables the inactivity detection.
 * @param inactAxes - Axes which participate in detecting inactivity.
 *					  Example: 0x0 - disables axes participation.
 *						  	   ADXL345_INACT_X_EN - enables x-axis.
 *							   ADXL345_INACT_Y_EN - enables y-axis.
 *							   ADXL345_INACT_Z_EN - enables z-axis.
 * @param inactAcDc - Selects dc-coupled or ac-coupled operation.
 *					  Example: 0x0 - dc-coupled operation.
 *							   ADXL345_INACT_ACDC - ac-coupled operation.
 * @param inactThresh - Threshold value for detecting inactivity.
 * @param inactTime - Inactivity time.
 * @patam inactInt - Interrupts pin.
 *				     Example: 0x0 - inactivity interrupts on INT1 pin.
 *							  ADXL345_INACTIVITY - inactivity interrupts on
 *												   INT2 pin.
 *
 * @return None.
*******************************************************************************/
void ADXL345_SetInactivityDetection(unsigned char inactOnOff,
									unsigned char inactAxes,
									unsigned char inactAcDc,
									unsigned char inactThresh,
									unsigned char inactTime,
									unsigned char inactInt)
{
	unsigned char oldActInactCtl = 0x0;
	unsigned char newActInactCtl = 0x0;
	unsigned char oldIntMap = 0x0;
	unsigned char newIntMap = 0x0;
	unsigned char oldIntEnable = 0x0;
	unsigned char newIntEnable = 0x0;
	oldActInactCtl = ADXL345_GetRegisterValue(ADXL345_INT_ENABLE);
	newActInactCtl = oldActInactCtl & ~(ADXL345_INACT_ACDC |
										ADXL345_INACT_X_EN |
										ADXL345_INACT_Y_EN |
										ADXL345_INACT_Z_EN);
	newActInactCtl = newActInactCtl | (inactAcDc | inactAxes);
	ADXL345_SetRegisterValue(ADXL345_ACT_INACT_CTL, newActInactCtl);
	ADXL345_SetRegisterValue(ADXL345_THRESH_INACT, inactThresh);
	ADXL345_SetRegisterValue(ADXL345_TIME_INACT, inactTime);
	oldIntMap = ADXL345_GetRegisterValue(ADXL345_INT_MAP);
	newIntMap = oldIntMap & ~(ADXL345_INACTIVITY);
	newIntMap = newIntMap | inactInt;
	ADXL345_SetRegisterValue(ADXL345_INT_MAP, newIntMap);
	oldIntEnable = ADXL345_GetRegisterValue(ADXL345_INT_ENABLE);
	newIntEnable = oldIntEnable & ~(ADXL345_INACTIVITY);
	newIntEnable = newIntEnable | (ADXL345_INACTIVITY * inactOnOff);
	ADXL345_SetRegisterValue(ADXL345_INT_ENABLE, newIntEnable);
}

/***************************************************************************//**
 * @brief Enables/disables the free-fall detection.
 *
 * @param ffOnOff - Enables/disables the free-fall detection.
 *					Example: 0x0 - disables the free-fall detection.
 *							 0x1 - enables the free-fall detection.
 * @param ffThresh - Threshold value for free-fall detection.
 * @param ffTime - Time value for free-fall detection.
 * @param ffInt - Interrupts pin.
 *				  Example: 0x0 - free-fall interrupts on INT1 pin.
 *						   ADXL345_FREE_FALL - free-fall interrupts on INT2 pin.
 *
 * @return None.
*******************************************************************************/
void ADXL345_SetFreeFallDetection(unsigned char ffOnOff,
								  unsigned char ffThresh,
								  unsigned char ffTime,
								  unsigned char ffInt)
{
	unsigned char oldIntMap = 0x0;
	unsigned char newIntMap = 0x0;
	unsigned char oldIntEnable = 0x0;
	unsigned char newIntEnable = 0x0;
	ADXL345_SetRegisterValue(ADXL345_THRESH_FF, ffThresh);
	ADXL345_SetRegisterValue(ADXL345_TIME_FF, ffTime);
	oldIntMap = ADXL345_GetRegisterValue(ADXL345_INT_MAP);
	newIntMap = oldIntMap & ~(ADXL345_FREE_FALL);
	newIntMap = newIntMap | ffInt;
	ADXL345_SetRegisterValue(ADXL345_INT_MAP, newIntMap);
	oldIntEnable = ADXL345_GetRegisterValue(ADXL345_INT_ENABLE);
	newIntEnable = oldIntEnable & ~ADXL345_FREE_FALL;
	newIntEnable = newIntEnable | (ADXL345_FREE_FALL * ffOnOff);
	ADXL345_SetRegisterValue(ADXL345_INT_ENABLE, newIntEnable);	
}

/***************************************************************************//**
 * @brief Calibrates the accelerometer.
 *
 * @param xOffset - X-axis's offset.
 * @param yOffset - Y-axis's offset.
 * @param zOffset - Z-axis's offset.
 *
 * @return None.
*******************************************************************************/
void ADXL345_SetOffset(unsigned char xOffset,
					   unsigned char yOffset,
					   unsigned char zOffset)
{
	ADXL345_SetRegisterValue(ADXL345_OFSX, xOffset);
	ADXL345_SetRegisterValue(ADXL345_OFSY, yOffset);
	ADXL345_SetRegisterValue(ADXL345_OFSZ, yOffset);
}
