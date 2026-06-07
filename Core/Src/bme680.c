/*
 * bme680.c
 *
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
 * 		Pinout
 * 			GND
 * 			NC
 * 			VCC
 * 			SCL/SCK
 * 			SDA/SDI
 * 			SDO
 * 			CS
 *
 *
 *  Created on: Jun 6, 2026
 *      Author: Roberto Bech
 */

#define BME680_ADDR_1 0x76  // SDO is set to GND
#define BME680_ADDR_2 0x77  // SDO is tet to VCC


#include "bme680.h"
