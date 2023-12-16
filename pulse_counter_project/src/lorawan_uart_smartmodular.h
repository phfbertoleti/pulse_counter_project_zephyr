/* Header file: smart modular (lorawan module) */
#include <zephyr/drivers/uart.h>

#ifndef HEADER_LORAWAN_UART
#define HEADER_LORAWAN_UART

/* UART parametrization */
#define BAUDRATE_LORAWAN_UART    9600
#define PARITY_LORAWAN_UART      UART_CFG_PARITY_NONE
#define STOP_BITS_LORAWAN_UART   UART_CFG_STOP_BITS_1
#define FLOW_CTRL_LORAWAN_UART   UART_CFG_FLOW_CTRL_NONE
#define DATA_BITS_LORAWAN_UART   UART_CFG_DATA_BITS_8

/* AT Commands */
#define AT_CMD_TERMINATOR        "\n"
#define AT_CMD_WAKE              "AT"
#define AT_CMD_RESET             "ATZ"
#define AT_CMD_JOIN_MODE_ABP     "AT+NJM=0"
#define AT_CMD_CLASS             "AT+CLASS=A"
#define AT_CMD_DEVADDR           "AT+DADDR"
#define AT_CMD_APPSKEY           "AT+APPSKEY"
#define AT_CMD_NWSKEY            "AT+NWKSKEY"
#define AT_CMD_APPEUI            "AT+APPEUI"
#define AT_CMD_SEND_BYTE         "AT+SENDB"

#endif

/* Prototypes */
int init_lorawan_smartmodular_uart(void);
void send_pulse_counter(int total_pulses);