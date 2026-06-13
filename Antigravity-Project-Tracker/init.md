# Project Initialization Blueprint: BME680 on STM32F401RE
## Configuration Reference & Setup Guide

This document captures the current state, peripheral parameters, and baseline code configurations for the STM32F401RE Nucleo-64 project. Use this as a reference to recreate or initialize the environment in STM32CubeMX and STM32CubeIDE.

---

## 1. Project Parameters & Environment

*   **Microcontroller:** STM32F401RET6 (Cortex-M4, 84 MHz maximum frequency)
*   **Sensor:** Bosch Sensortec BME680 (Environmental/Gas sensor via I2C)
*   **Real-Time OS:** FreeRTOS (CMSIS-RTOS V2 interface)
*   **IDE/Toolchain:** STM32CubeMX + STM32CubeIDE (GCC compiler)
*   **System Clock:** Internal HSI (16 MHz) multiplied to **84 MHz** via PLL

---

## 2. Hardware Peripheral Setup (STM32CubeMX Configuration)

### A. I2C1 Configuration
*   **Mode:** I2C
*   **Speed Mode:** Standard Mode (100 kHz)
*   **Clock Speed:** 100000 Hz
*   **Duty Cycle:** 2 (Standard for Standard Mode)
*   **Pin Mapping:**
    *   `PB8` -> I2C1_SCL (Arduino Connector D15)
    *   `PB9` -> I2C1_SDA (Arduino Connector D14)
*   **GPIO Parameters:** Output speed set to **High**, pull-up resistors enabled (if not present on breakout board).

### B. USART2 Configuration (Debug Console)
*   **Mode:** Asynchronous
*   **Baud Rate:** 115200 Bits/s
*   **Word Length:** 8 bits (including parity)
*   **Stop Bits:** 1
*   **Parity:** None
*   **Pin Mapping:**
    *   `PA2` -> USART2_TX (Connected directly to ST-LINK Virtual COM Port)
    *   `PA3` -> USART2_RX (ST-LINK Virtual COM Port)

### C. System Core & Timebase Setup (SYS)
*   **Debug:** Serial Wire
*   **Timebase Source:** `TIM5` (Required: Do **NOT** use `SysTick` since FreeRTOS claims it).

### D. FreeRTOS Middleware
*   **API version:** CMSIS_V2
*   **Memory Management:** Dynamic allocation (Heap_4 method recommended).
*   **Default Task (`defaultTask`):**
    *   Priority: `osPriorityNormal`
    *   Stack Size: 128 words (512 bytes)
    *   Entry Function: `StartDefaultTask`

---

## 3. Baseline Code Configurations (STM32CubeIDE Setup)

### A. Printf Redirection (to USART2)
To direct standard output (`printf`) to the serial terminal, the following implementation is added to `main.c` under `/* USER CODE BEGIN 0 */` or `/* USER CODE BEGIN PFP */`:

```c
#include <stdio.h>

/* Redirection of standard output (stdout) to USART2 */
int __io_putchar(int ch) {
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

int _write(int file, char *ptr, int len) {
    HAL_UART_Transmit(&huart2, (uint8_t *)ptr, len, HAL_MAX_DELAY);
    return len;
}
```

### B. Heartbeat Task Implementation
Inside `main.c`, the default FreeRTOS thread triggers a heartbeat to indicate scheduler status:

```c
void StartDefaultTask(void *argument) {
  /* USER CODE BEGIN StartDefaultTask */
  printf("System Scheduler Started!\r\n");
  /* Infinite loop */
  for(;;) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); // Toggle Onboard LED (LD2)
    printf("[Heartbeat] System active...\r\n");
    osDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}
```

---

## 4. Initialization & Deployment Steps

1.  **Hardware Connection:**
    *   Connect BME680 SCL to Nucleo pin `PB8` and SDA to `PB9`.
    *   Connect VCC to `3.3V` and GND to `GND`.
2.  **Importing Driver:**
    *   Add official `bme680.c` and `bme680.h` files to target directories.
3.  **Low-Level Wrapper Setup:**
    *   Map `HAL_I2C_Mem_Read` and `HAL_I2C_Mem_Write` to BME680 sensor initialization structures.

---

## 5. Completed Tasks and Current Status

*   **Phase 1 (Setup):** **[COMPLETE]** Initialize CubeMX, configure peripherals, select clock configuration, configure FreeRTOS, and generate the project source code.
*   **Phase 2 (Baseline):** **[COMPLETE]** Code import, standard I/O redirection setup, compilation testing, and verification of the scheduler heartbeat.
*   **Phase 3 (Physical I2C):** **[COMPLETE]** Sensor physical connections verified and communication established by successfully reading Chip ID (0x61).
*   **Phase 4 (RTOS Tasks & IPC):** **[COMPLETE]** Multi-task environment fully functional. BME680Task extracts native sensor data structures and sends them via `xQueueSend()` to MainTask, which blocks on `xQueueReceive()` and formats the print outputs to USART2.
