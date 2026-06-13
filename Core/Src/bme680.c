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

#include "bme68x.h"
#include "bme680.h"
#include "usart.h"
#include <stdbool.h>
#include <stdlib.h>

struct bme68x_dev bme;
struct bme68x_conf conf;
struct bme68x_heatr_conf heatr_conf;
struct bme68x_data data;

static uint8_t dev_addr;

/**
 * @brief Initialize BME680
 */
void bme680_init(void)
{
	// Interface updated for I2C
	int8_t rslt = bme68x_interface_init(&bme, BME68X_I2C_INTF);
	bme68x_check_rslt("bme68x_interface_init", rslt);
	printf("BME680: I2C Initialized\r\n");

	// initialize BME680 driver
	rslt = bme68x_init(&bme);
	bme68x_check_rslt("bme68x_init", rslt);
	printf("BME680: Chip ID %u\r\n", bme.chip_id);

	/* Check if rslt == BME68X_OK, report or handle if otherwise */
	conf.filter = BME68X_FILTER_OFF;
	conf.odr = BME68X_ODR_NONE;
	conf.os_hum  = BME68X_OS_2X;
	conf.os_pres = BME68X_OS_1X;
	conf.os_temp = BME68X_OS_2X;
	rslt = bme68x_set_conf(&conf, &bme);
	bme68x_check_rslt("bme68x_set_conf", rslt);
	printf("BME680: Configuration set\r\n");

	/* Check if rslt == BME68X_OK, report or handle if otherwise */
	heatr_conf.enable = BME68X_ENABLE;
	heatr_conf.heatr_temp = 300;
	heatr_conf.heatr_dur = 100; // duration in milliseconds
	rslt = bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &bme);
	bme68x_check_rslt("bme68x_set_heatr_conf", rslt);
	printf("BME680: Heater configured\r\n");

}

void bme680_get_measurement(void)
{
	uint32_t del_period;
	uint8_t n_fields;
	uint8_t rslt = bme68x_set_op_mode(BME68X_FORCED_MODE, &bme);
	bme68x_check_rslt("bme68x_set_op_mode", rslt);

	/* Calculate delay period in microseconds */
	// Header duration is in milliseconds - convert to us with *1000
	del_period = bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &bme) + (heatr_conf.heatr_dur * 1000);
	bme.delay_us(del_period, bme.intf_ptr);

	/* Check if rslt == BME68X_OK, report or handle if otherwise */
	rslt = bme68x_get_data(BME68X_FORCED_MODE, &data, &n_fields, &bme);
	bme68x_check_rslt("bme68x_get_data", rslt);

    if (n_fields)
    {
#ifdef BME68X_USE_FPU
        printf("Temp: %.2f, RH: %.2f, Pressure (Pa): %.2f, GasR: %.2f, Status: 0x%u\r\n",
               data.temperature,
               data.humidity,
			   (data.pressure - 101325), // remove atmospheric pressure in Pascals
               data.gas_resistance,
               data.status);
#else
        printf("%d, %lu, %lu, %lu, 0x%x\r\n",
               (data.temperature / 100),
               (long unsigned int)data.pressure,
               (long unsigned int)(data.humidity / 1000),
               (long unsigned int)data.gas_resistance,
               data.status);
#endif
    }
}

/**
 * @brief Get the BME680 ID
 * @return[out] The unsigned 8-bit ID of the BME680
 */
uint8_t bme680_get_id(void)
{
	uint8_t rx_buffer[1];

	I2C_Read(BME680_ADDRESS, BME68X_CHIP_ID, rx_buffer, 1);

	return rx_buffer[0];
}

/*!
 * @brief I2C read function wrapper
 * @param[in] reg_addr - target device address
 * @param[in] *reg_data - buffer for the data to be received
 * @param[in] len - The size of the data to be received
 * @param[in] *intf_ptr - pointer to driver data - contains BME680 device address
 */
BME68X_INTF_RET_TYPE bme68x_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
	BME68X_INTF_RET_TYPE ret = BME68X_OK;
	uint8_t device_addr = *(uint8_t*)intf_ptr;
    (void)intf_ptr;

    //return coines_read_i2c(COINES_I2C_BUS_0, device_addr, reg_addr, reg_data, (uint16_t)len);

    // Perform I2C Read
    if(I2C_Read(device_addr, reg_addr, reg_data, len))
    {
    	// success
    	ret = BME68X_INTF_RET_SUCCESS;
    }
    else
    {
    	ret = BME68X_E_COM_FAIL;
    }

    return ret;
}

/*!
 * I2C write function wrapper
 */
BME68X_INTF_RET_TYPE bme68x_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
	BME68X_INTF_RET_TYPE ret = BME68X_OK;
    uint8_t device_addr = *(uint8_t*)intf_ptr;

    (void)intf_ptr;

    //return coines_write_i2c(COINES_I2C_BUS_0, device_addr, reg_addr, (uint8_t *)reg_data, (uint16_t)len);

    if(I2C_Write(device_addr, reg_addr, (uint8_t*)reg_data, len))
    {
    	// success
    	ret = BME68X_INTF_RET_SUCCESS;
    }
    else
    {
    	ret = BME68X_E_COM_FAIL;
    }

    return ret;
}

/*!
 * @brief Delay function map to COINES platform
 * @param[in] period - time in microseconds
 * @param[in] *intf_ptr - pointer to interface
 */
void bme68x_delay_us(uint32_t period, void *intf_ptr)
{
    (void)intf_ptr;

    // Convert to ms
    uint32_t ms = (uint32_t)(period / 1000);

    //printf("BME680 Delay Period: %lu ms\r\n", ms);

    vTaskDelay(pdMS_TO_TICKS(ms));
}

int8_t bme68x_interface_init(struct bme68x_dev *bme, uint8_t intf)
{
    int8_t rslt = BME68X_OK;

    // null guard - check if initialized
    if (bme != NULL)
    {
        /* Bus configuration : I2C */
        if (intf == BME68X_I2C_INTF)
        {
            printf("I2C Interface\r\n");
            dev_addr = BME68X_I2C_ADDR_LOW;
            bme->read = bme68x_i2c_read;
            bme->write = bme68x_i2c_write;
            bme->intf = BME68X_I2C_INTF;
        }
        /* Bus configuration : SPI */
        else if (intf == BME68X_SPI_INTF)
        {
        	// not using SPI - leave it just in case
        	/*
            printf("SPI Interface\n");
            dev_addr = COINES_SHUTTLE_PIN_7;
            bme->read = bme68x_spi_read;
            bme->write = bme68x_spi_write;
            bme->intf = BME68X_SPI_INTF;
            (void)coines_config_spi_bus(COINES_SPI_BUS_0, COINES_SPI_SPEED_7_5_MHZ, COINES_SPI_MODE0);
            */
        }

        bme->delay_us = bme68x_delay_us;
        bme->intf_ptr = &dev_addr;
        bme->amb_temp = 25; /* The ambient temperature in deg C is used for defining the heater temperature */
    }
    else
    {
        rslt = BME68X_E_NULL_PTR;
    }

    return rslt;
}

void bme68x_check_rslt(const char api_name[], int8_t rslt)
{
    switch (rslt)
    {
        case BME68X_OK:

            /* Do nothing */
            break;
        case BME68X_E_NULL_PTR:
            printf("API name [%s]  Error [%d] : Null pointer\r\n", api_name, rslt);
            break;
        case BME68X_E_COM_FAIL:
            printf("API name [%s]  Error [%d] : Communication failure\r\n", api_name, rslt);
            break;
        case BME68X_E_INVALID_LENGTH:
            printf("API name [%s]  Error [%d] : Incorrect length parameter\r\n", api_name, rslt);
            break;
        case BME68X_E_DEV_NOT_FOUND:
            printf("API name [%s]  Error [%d] : Device not found\r\n", api_name, rslt);
            break;
        case BME68X_E_SELF_TEST:
            printf("API name [%s]  Error [%d] : Self test error\r\n", api_name, rslt);
            break;
        case BME68X_W_NO_NEW_DATA:
            printf("API name [%s]  Warning [%d] : No new data found\r\n", api_name, rslt);
            break;
        default:
            printf("API name [%s]  Error [%d] : Unknown error code\r\n", api_name, rslt);
            break;
    }
}

