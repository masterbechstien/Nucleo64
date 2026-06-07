/*
 * bme680.h
 *
 *  Created on: Jun 6, 2026
 *      Author: maste
 */

#ifndef INC_BME680_H_
#define INC_BME680_H_

#include "common.h"
#include "i2c.h"

#define BME680_ADDR_1 0x76  // SDO is set to GND
#define BME680_ADDR_2 0x77  // SDO is tet to VCC

#define BME680_ADDRESS BME680_ADDR_1 // SDO is connected to GND

void bme680_init(void);

void bme680_set_humidity_oversampling(void);
void bme680_set_temperature_oversampling(void);
void bme680_set_pressure_oversampling(void);

#endif /* INC_BME680_H_ */
