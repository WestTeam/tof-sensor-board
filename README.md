# tof-sensor-board

[![Build Status](https://travis-ci.com/WestTeam/tof-sensor-board.svg?branch=master)](https://travis-ci.com/WestTeam/tof-sensor-board)

A project around ST VL6180X tof sensor

This project use ChibiOS on stable branch 18.2.x and runs on board from CVRA:
https://github.com/cvra/sensor-board

The tag STM32L476 has a working sample project running on the Nucleo STM32L476RG board and below is the wiring on the demo board.

HW wiring:
==
  * Use PWM2 CH1 (PA5) for status LED
  * Use I2C2 (PB13/PB14) for VL6180x sensor
  * Use USART2 (PA2/PA3) for CLI
  * Use USART3 (PB10/PB11) for data logging

For both project, the build/flash process is really simple and easy:

Build:
==

```
make -j7
```

Flash:
==

```
make -j7 flash
```
