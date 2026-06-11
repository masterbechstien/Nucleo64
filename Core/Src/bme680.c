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
#include <stdlib.h>

// List of gas ranges and corresponding constants used for the resistance calculation
// Constants to be integrated into the driver
static float const_array1[16] = {1, 1, 1, 1, 1, 0.99, 1, 0.992, 1, 1, 0.998, 0.995, 1, 0.99, 1, 1};
static float const_array2[16] = {8000000,4000000,2000000,1000000,499500.4995,248262.1648,125000,63004.03226,31281.28128,15625,7812.5,3906.25,1953.125,976.5625,488.28125,244.140625};
static uint32_t const_array1_int[16] = {2147483647,2147483647,2147483647,2147483647,2147483647,2126008810,2147483647,2130303777,2147483647,2147483647,2143188679,2136746228,2147483647,2126008810,2147483647,2147483647};
static uint32_t const_array2_int[16] = {4096000000,2048000000,1024000000,512000000,255744255,127110228,64000000,32258064,16016016,8000000,4000000,2000000,1000000,500000,250000,125000};

/**
 * @brief Initialize BME680
 */
void bme680_init(void)
{
	// Check I2C if the device is available

	if(I2C_IsDeviceReady(BME680_ADDRESS))
	{
		// BME680 is available

		printf("BME680 is ready\r\n");

		// configure the sensors
		bme680_config_sensors();
	}
	else
	{
		printf("BME680 is not ready");
	}

}

/**
 * @brief Wrapper function for sending and receiving I2C communication
 */
/**
 * @brief Wrapper function for handling transmit and receive I2C commands
 * @param[in] *tx_buffer - the buffer array to be transmitted
 * @param[in] size - how many bytes to transmit
 * @param[in] perform_receive - True: execute master receive, False: Do not execute master receive
 * Note: The tx_buffer must contain the register address
 */
void bme680_i2c_command(uint8_t *tx_buffer, uint8_t size, bool perform_receive)
{
	// send I2C read request
	I2C_MasterTransmit(BME680_ADDRESS, tx_buffer, 1);


	// If flag is true then a response from BME680 it is expected so perform read
	if(!perform_receive)
	{
		// clear tx_buffer
		for(uint8_t i = 0; i < size; i++)
		{
			tx_buffer[i] = 0;
		}

		// perform read - store results in the tx_buffer
		I2C_MasterReceive(BME680_ADDRESS, tx_buffer, 2);
	}
}

/**
 * @brief Configure the BME680 temperature, RH, and pressure for 2x oversampling and put into forced mode
 * Note: BME680 Datasheet - Page 15
 */
void bme680_config_sensors(void)
{
	// On power-on the BME680 is in low power mode, so it will need to be configured to 'forced mode' to start taking measurements

	// 1. Create the buffers for TX and RX
	// BME680 Datasheet - page 15 - "it is highly recommended to set first osrs_h<2:0> followed by osrs_t<2:0> and osrs_p<2:0> in one write command"

	uint8_t *tx_buffer = (uint8_t*)malloc(* sizeof(uint8_t));
	uint8_t *rx_buffer= (uint8_t*)malloc(1*sizeof(uint8_t));

	// 2. set bits for oversampling 2x for temp, rh, and pressure

	// 3. set temp and pressure to forced mode

	// 4. Transmit

}

/**
 * @brief Get the BME680 ID
 * @return[out] The unsigned 8-bit ID of the BME680
 */
uint8_t bme680_get_id(void)
{
	uint8_t *tx_buffer = (uint8_t*)malloc(1 * sizeof(uint8_t));
	uint8_t *rx_buffer = (uint8_t*)malloc(1 * sizeof(uint8_t));

	tx_buffer[0] = BME680_REG_ID;
	rx_buffer[0] = DEFAULT_TXRX_VALUE;

	// send I2C read request
	I2C_MasterTransmit(BME680_ADDRESS, tx_buffer, 1);

	// perform read - store results in the tx_buffer
	I2C_MasterReceive(BME680_ADDRESS, rx_buffer, 1);

	return *rx_buffer;
}

/**
 * @brief Set the BME680 mode to sleep mode - low power mode - no measurements will be made
 */
void bme680_sleep_mode(void)
{
	// mode<1:0> == 00
	//uint8_t *buffer = (uint8_t *) malloc(2 * sizeof(uint8_t)); // allocate memory for 2 element

	//buffer[0] = cmd; // 1. the register to write to
	//buffer[1] = 0x00; // 2. The value to set the register to

	//bme680_i2c_command(buffer, I2C_READ, false);
}

/**
 * @brief Set the BME680 mode to forced mode - Measurements will be made
 */
void bme680_forced_mode(void)
{
	// mode<1:0> == 01
}


