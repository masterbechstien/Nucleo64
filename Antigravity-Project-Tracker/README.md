# STM32F401RE Nucleo-64 BME680 FreeRTOS Project

To resume the Antigravity pair programming session where we left off, run the following command in this directory (this resumes your previous persistent session):
```bash
agy -C
```

---

This repository contains the embedded C project code and development tracking for integrating a **Bosch BME680** environmental sensor with an **STM32F401RE microcontroller** (on a Nucleo-64 board) using I2C communication under the **FreeRTOS** operating system.

---

## 📂 Workspace Reference Files

To help you manage and track the development process, the workspace contains two core documentation files:
1.  **[ProjectOutline.md](file:///home/master/Nucleo64/ProjectOutline.md) (Roadmap & Gantt):** Tracks project phases, milestones, timelines, and task dependencies using Mermaid Gantt and flow charts.
2.  **[init.md](file:///home/master/Nucleo64/init.md) (Configuration Blueprint):** Documenting peripheral settings (I2C1, USART2, SYS Timebase), default task parameter configurations, and setup code snippets.

---

## 🚀 How to Resume Development with Antigravity

When you open this workspace in the future and want to continue working, follow these instructions to bring your AI pair programmer up to speed:

### 1. Launch Antigravity
Open the command line or terminal inside this project directory (`/home/master/Nucleo64`) and start the Antigravity session.

### 2. Instruct the Agent to Catch Up
Send the following prompt to Antigravity as your first message:

```text
Read init.md and ProjectOutline.md to see where we left off, and let's continue with Phase 3 (writing the custom HAL I2C wrappers).
```

This instruction allows Antigravity to parse the hardware configuration, locate the baseline code, read the flowchart status, and seamlessly resume coding the driver wrappers.

---

## 📌 Hardware Connections Reference

| BME680 Pin | Nucleo-64 Connector Pin | MCU Pin | Description |
| :--- | :--- | :--- | :--- |
| **VCC** | CN6 Pin 4 | - | 3.3V Power Supply |
| **GND** | CN6 Pin 6 | - | Ground Reference |
| **SCL** | D15 (CN5 Pin 10) | `PB8` | I2C1 Serial Clock (Requires Pull-up) |
| **SDA** | D14 (CN5 Pin 9) | `PB9` | I2C1 Serial Data (Requires Pull-up) |

---

## 📈 Current Project Status (Summary)

*   **Phase 1 (Hardware Setup):** Complete (CubeMX code generated).
*   **Phase 2 (Baseline Code):** Complete (Printf redirected to USART2, heartbeat LED blinking under RTOS).
*   **Phase 3 (I2C & Driver Integration):** **Complete** (Physical wiring verified and BME680 Chip ID read successfully).
*   **Phase 4 (RTOS Tasks & IPC):** **Complete** (BME680Task sends driver data structures via queue to MainTask, which prints logs to USART2).
