/*
 * bme680.h
 *
 *  Created on: Jun 6, 2026
 *      Author: Roberto Bech
 *
 *      site: https://esp-idf-lib.readthedocs.io/en/latest/groups/bme680.html
 */

#ifndef INC_BME680_H_
#define INC_BME680_H_

#include "common.h"
#include "i2c.h"

#define MAX_NUMBER_REGISTERS 32
#define DEFAULT_TXRX_VALUE 0

#define BME680_ADDR_1 0x76  // SDO is set to GND
#define BME680_ADDR_2 0x77  // SDO is tet to VCC
#define BME680_ADDRESS BME680_ADDR_1 // SDO is connected to GND

#define OVERSAMPLING_MASK 7 // First 3 bits of Temp osrs_t<2:0>, RH osrs_r<2:0>, and Pressure osrs_p<2:0>

#define OVERSAMPLING_1X  1
#define OVERSAMPLING_2X  2
#define OVERSAMPLING_4X  4
#define OVERSAMPLING_8X  8
#define OVERSAMPLING_16X 16

#define BME680_REG_STATUS       0x73  // Bit<4> spi_mem_page
#define BME680_REG_RESET		0xE0  // Bit<7:0> reset
#define BME680_REG_ID			0xD0  // Bit<7:0> chip_id
#define BME680_REG_CONFIG		0x75  // Bit<4:2> filter, Bit<0> spi_3w_en
#define BME680_REG_CTRL_MEAS	0x74  // Bit<7:5> osrs_t, Bit<4:2> osrs_p, Bit<1:0> mode
/**
 * osrs_t<7:5>     | Temperature oversampling
 *     000         | Skipped (output set to 0x8000)
 *     001         |  oversampling x1
 *     010         |  oversampling x2
 *     011         |  oversampling x4
 *     100         |  oversampling x8
 *     101, others |  oversampling x16
 */

/**
 * osrs_p<4:2>     | Temperature oversampling
 *     000         | Skipped (output set to 0x8000)
 *     001         |  oversampling x1
 *     010         |  oversampling x2
 *     011         |  oversampling x4
 *     100         |  oversampling x8
 *     101, others |  oversampling x16
 */
#define BME680_REG_CTRL_HUM		0x72  // Bit<6> spi_3w_int_en, Bit<2:0> osrs_h
/**
 * osrs_h<2:0>     | Humidity oversampling
 *     000         | Skipped (output set to 0x8000)
 *     001         |  oversampling x1
 *     010         |  oversampling x2
 *     011         |  oversampling x4
 *     100         |  oversampling x8
 *     101, others |  oversampling x16
 */
#define BME680_REG_CTRL_GAS_1	0x71  // Bit<4> run_gas, Bit<3:0> nb_conv
#define BME680_REG_CTRL_GAS_0	0x70  // Bit<3> heat_off
#define BME680_REG_GAS_WAIT_X	0x64  // 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D
#define BME680_REG_RES_HEAT_X   0x5A  // 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x62, 0x63
#define BME680_REG_IDAC_HEAT_X	0x50  // 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59
#define BME680_REG_GAS_R_LSB	0x2B
#define BME680_REG_GAS_R_MSB	0x2A
#define BME680_REG_HUM_LSB		0x26
#define BME680_REG_HUM_MSB		0x25
#define BME680_REG_TEMP_XLSB	0x24
#define BME680_REG_TEMP_LSB		0x23
#define BME680_REG_TEMP_MSB		0x22
#define BME680_REG_PRESS_XLSB	0x21
#define BME680_REG_PRESS_LSB	0x20
#define BME680_REG_PRESS_MSB	0x1F
#define BME680_REG_EAS_STATUS_0 0x1D

/**
 *  Oversampling - Number of samples to take
 *  Increases resolution with larger values - this reduces noise
 *  Larger values also reduces how often measurements can be requested
 *  1, 2, 4, 8, or 16
 */

#define I2C_WRITE 0
#define I2C_READ  1

#define TEMP_OVERSAMPLING 2
#define RH_OVERSAMPLING 2
#define PRES_OVERSAMPLING 2

void bme680_init(void);
void bme680_forced_mode(void);
void bme680_sleep_mode(void);
uint8_t bme680_get_id(void);

void bme680_set_humidity_oversampling(void);
void bme680_set_temperature_oversampling(void);
void bme680_set_pressure_oversampling(void);

void bme680_config_sensors(void);
void bme680_i2c_command(uint8_t *tx_buffer, uint8_t size, bool perform_receive);


#endif /* INC_BME680_H_ */
