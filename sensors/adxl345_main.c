/***************************************************************************//**
 *   @file   Main.c
 *   @brief  Implementation of the program's main function.
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
#include <stdio.h>
#include "freeRTOS.h"

#include "ADXL345.h"

#define DEBUG_PRINT  printf
/******************************************************************************/
/* Variables Declarations                                                     */
/******************************************************************************/
unsigned char 	periodEnd	= 0;	// Indicates when the OneShot period ends.
unsigned short	x			= 0;	// X-axis's output data.
unsigned short	y			= 0;	// Y-axis's output data.
unsigned short	z 			= 0;	// Z-axis's output data.
unsigned char	intSource	= 0;	// Value of the ADXL345_INT_SOURCE register.

/******************************************************************************/
/* Functions Prototypes                                                       */
/******************************************************************************/
void DelaySeconds(float seconds);
void OneShotCallback(void);

/***************************************************************************//**
 * @brief Main function.
 *
 * @param None.
 *
 * @return None.
*******************************************************************************/
void adxl345(void)
{
	if(ADXL345_Init())
	{
//	  DEBUG_PRINT("ADXL345 OK\n");
	}
	else
	{
	  DEBUG_PRINT("ADXL345 Err\n");
	}

#if 0
	ADXL345_SetTapDetection(ADXL345_SINGLE_TAP |
							ADXL345_DOUBLE_TAP,	// Tap type.
							ADXL345_TAP_X_EN,	// Axis control.
							0x10,				// Tap duration.
							0x10,				// Tap latency.
							0x40,				// Tap window. 
							0x10,				// Tap threshold.
							0x00);				// Interrupt Pin.
	ADXL345_SetFreeFallDetection(0x01,	// Free-fall detection enabled.
								 0x05,	// Free-fall threshold.
								 0x14,	// Time value for free-fall detection.
								 0x00);	// Interrupt Pin.
#endif

	ADXL345_SetPowerMode(0x1);			// Measure mode.


  vTaskDelay(pdMS_TO_TICKS(3000));

	for (int i = 0; i < 1000; i++) {
    int16_t x,y,z;

    vTaskDelay(pdMS_TO_TICKS(1));

    ADXL345_GetXyz(&x,&y,&z);

    DEBUG_PRINT("%d\r\n", x);
	}

	while (1) ;

	while(1)
	{
	  int16_t x,y,z;

    vTaskDelay(pdMS_TO_TICKS(100));

		ADXL345_GetXyz(&x,&y,&z);

//    DEBUG_PRINT("X: %6d, Y: %6d, Z: %6d\n", x, y, z);

    DEBUG_PRINT("%6d %6d %6d\r\n", x, y, z);

#if 0
		intSource = ADXL345_GetRegisterValue(ADXL345_INT_SOURCE);
		if((intSource & ADXL345_SINGLE_TAP) != 0)
		{
		  DEBUG_PRINT("Single Tap\n");
		}
		if((intSource & ADXL345_DOUBLE_TAP) != 0)
		{
		  DEBUG_PRINT("Double Tap\n");
		}
		if((intSource & ADXL345_FREE_FALL) != 0)
		{
		  DEBUG_PRINT("Free-Fall\n");
		}
#endif
	}
}

/***************************************************************************//**
 * @brief OneShot callback function.
 *
 * @param None.
 *
 * @return None.
*******************************************************************************/
void OneShotCallback(void)
{
	periodEnd = 1;
}
