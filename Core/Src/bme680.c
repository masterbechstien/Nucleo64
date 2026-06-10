/*
 * bme680.c
 *  Created on: Jun 6, 2026
 *      Author: Roberto Bech
 *
 * Sensor Description: Low power gas, pressure, temperature & humidity sensor
 *
 * site: https://esp-idf-lib.readthedocs.io/en/latest/groups/bme680.html
 * site: https://github.com/watterott/BME680-Breakout
 * The communication protocol will be set to I2C.
 * For I2C to be utilized, the datasheet states that "If CSB is connected to VDDIO, the I2C interface is active".
 * The breakout board labels CSB as "CS" and automatically sets it to VCC with a pull-up resistor.
 *
 * There are two 7-bit device addresses available and the user can select the address by either setting SDO to VCC or GND
 * 		Address | SDO
 * 		------- | -----
 * 		  0x76  | GND
 * 		  0x77  | VCC
 *
 * Hardware Setup
 * BME680 breakout
 * 		Pinout        |  Description         |   I2C Connection
 * 		-----------   |  ------------------  |   --------------
 * 			GND       |   Ground             |    GND
 * 			NC        |   Not Connected      |    NC
 * 			VCC       |   Power              |    VCC
 * 			SCL/SCK   |   Serial Clock Input |    SCL
 * 			SDA/SDI   |   Serial Data Input  |    SDA
 * 			SDO       |   Serial Data Output |    GND for default address (0x76)
 * 			CS        |   chip select        |    VCC (enable I2C)
 *
 *
 *
 */


#include "bme680.h"
#include "usart.h"
#include <stdbool.h>

/**
 * @brief Initialize BME680
 */
void bme680_init(void)
{
	// Check I2C if the device is available

	if(I2C_IsDeviceReady(BME680_ADDRESS))
	{
		printf("BME680 is ready\r\n");
	}
	else
	{
		printf("BME680 is not ready");
	}

}

void bme680_sleep_mode(void)
{
	// mode<1:0> == 00
}

void bme680_forced_mode(void)
{
	// mode<1:0> == 01
}

