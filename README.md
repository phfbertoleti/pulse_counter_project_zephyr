# Pulse counter project

## Overview

This repo contains pulse counter project. This project is able to count pulses and send the counter value to cloud using a LoRaWAN module, attached to microcontroller's UART. The LoRaWAN module used here (Smart Modular - https://ziliatech.com/iot/) bahaves like a modem, controlled via AT commands.

The LoRaWAN communication uses ABP as activation mode, without package receive confirmation.

## Supported microcontrollers

This project has device overlays for:

* ESP32 WROOM (DevKitC) - xtensa architecture
* ESP32-C3 - RISC-V architecture
* RP2040 (rpi_pico) - ARM architecture

Therefore, this project is ready to be compiled for all these 

## How to compile and flash

Assuming this code is placed into zephyrproject/zephyr folder, you can compile it by doing the following: 

* For ESP32 WROOM, use the following command for compiling this project:

`west build -p always -b esp32_devkitc_wroom pulse_counter_project_zephyr`

* For ESP32-C3, use the following command for compiling this project:

`west build -p always -b esp32c3_devkitm pulse_counter_project_zephyr`

* For RP2040, use the following command for compiling this project:

`west build -p always -b rpi_pico pulse_counter_project_zephyr`

And for flashing, simply use:

`west flash`
