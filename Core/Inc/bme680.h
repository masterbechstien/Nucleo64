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
#include "bme68x_defs.h"
#include "bme68x.h"

#define BME680_ADDRESS BME68X_I2C_ADDR_LOW // BME68X_I2C_ADDR_LOW: SDO is connected to GND, BME68X_I2C_ADDR_HIGH: SDO connected to VCC


void bme680_init(void);

/*!
 *  @brief Function to select the interface between SPI and I2C.
 *
 *  @param[in] bme      : Structure instance of bme68x_dev
 *  @param[in] intf     : Interface selection parameter
 *
 *  @return Status of execution
 *  @retval 0 -> Success
 *  @retval < 0 -> Failure Info
 */
int8_t bme68x_interface_init(struct bme68x_dev *bme, uint8_t intf);

/*!
 *  @brief Function for reading the sensor's registers through I2C bus.
 *
 *  @param[in] reg_addr     : Register address.
 *  @param[out] reg_data    : Pointer to the data buffer to store the read data.
 *  @param[in] len          : No of bytes to read.
 *  @param[in] intf_ptr     : Interface pointer
 *
 *  @return Status of execution
 *  @retval = BME68X_INTF_RET_SUCCESS -> Success
 *  @retval != BME68X_INTF_RET_SUCCESS  -> Failure Info
 *
 */
BME68X_INTF_RET_TYPE bme68x_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);

/*!
 *  @brief Function for writing the sensor's registers through I2C bus.
 *
 *  @param[in] reg_addr     : Register address.
 *  @param[in] reg_data     : Pointer to the data buffer whose value is to be written.
 *  @param[in] len          : No of bytes to write.
 *  @param[in] intf_ptr     : Interface pointer
 *
 *  @return Status of execution
 *  @retval = BME68X_INTF_RET_SUCCESS -> Success
 *  @retval != BME68X_INTF_RET_SUCCESS  -> Failure Info
 *
 */
BME68X_INTF_RET_TYPE bme68x_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr);

/*!
 * @brief This function provides the delay for required time (Microsecond) as per the input provided in some of the
 * APIs.
 *
 *  @param[in] period       : The required wait time in microsecond.
 *  @param[in] intf_ptr     : Interface pointer
 *
 *  @return void.
 *
 */
void bme68x_delay_us(uint32_t period, void *intf_ptr);

/*!
 *  @brief Prints the execution status of the APIs.
 *
 *  @param[in] api_name : Name of the API whose execution status has to be printed.
 *  @param[in] rslt     : Error code returned by the API whose execution status has to be printed.
 *
 *  @return void.
 */
void bme68x_check_rslt(const char api_name[], int8_t rslt);

/**
 * @brief Poll for sensor measurements and print USART
 * @param N/A
 */
void bme680_poll_measurement(void);

/**
 * @brief Copy the current sensor measurements
 * @param N/A
 * @return[out] The BME680 data struct that holds teh sensor measurements
 */
void bme680_get_measurements(struct bme68x_data *pData);

/**
 * @brief Poll BME680 for the Chip ID - this is for debugging purposes
 * @param N/A
 * @return[out] 8bit  Chip ID
 */
uint8_t bme680_get_id(void);


#endif /* INC_BME680_H_ */
