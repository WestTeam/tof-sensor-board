# tof-sensor-board
A project around ST VL6180X tof sensor

This project use ChibiOS on stable branch 18.2.x and runs on the Nucleo
STM32L476RG board.

HW wiring:
==
  * Use PWM2 CH1 (PA5) for status LED
  * Use I2C2 (PB13/PB14) for VL6180x sensor
  * Use USART2 (PA2/PA3) for CLI
  * Use USART3 (PB10/PB11) for data logging

Build:
==

To build the project, you just need to type make at root dir.

Flash:
==

To flash the board, you just need to type make flash at root dir.
