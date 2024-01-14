#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <stdio.h>
#include <string.h>
#include <zephyr/logging/log.h>
#include "lorawan_uart_smartmodular.h"

LOG_MODULE_DECLARE(pulse_counter_lorawan);

/* Define time to receive response from smart modular lorawan module */
#define TIME_TO_RECEIVE_RESPONSE   1000 //ms

/* Define LoRaWAN Port (for LoRaWAN communication) */
#define LORAWAN_PORT               12

/* Load device tree defs for lorawan uart */
const struct device *uart_lorawan = DEVICE_DT_GET(DT_NODELABEL(uart1));

/* LoRaWAN ABP credentials */
static const char DEVADDR[] = "00:00:00:00";
static const char APPSKEY[] = "00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00";
static const char NWKSKEY[] = "00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00";
static const char APPEUI[] = "00:00:00:00:00:00:00:00";

/* Struct for lorawan uart configuration */
struct uart_config uart_cfg = {
	.baudrate = 9600,
	.parity = PARITY_LORAWAN_UART,
	.stop_bits = STOP_BITS_LORAWAN_UART,
	.flow_ctrl = FLOW_CTRL_LORAWAN_UART,
	.data_bits = DATA_BITS_LORAWAN_UART,
};

/* Local functions prototypes */
void wake_up_lorawan_smartmodular_uart(void);
void reset_lorawan_smartmodular_uart(void);
void join_mode_abp_smartmodular_uart(void);
void class_A_smartmodular_uart(void);
void device_address_smartmodular_uart(void);
void appskey_smartmodular_uart(void);
void nwskey_smartmodular_uart(void);
void appeui_smartmodular_uart(void);
void send_custom_at_command_lorawan_smartmodular_uart(char *at_command);
void rcv_response_lorawan_smartmodular_uart(void);

/* Function: init smart modular lorawan module uart communication
 * Params: none
 * Return: 0: error
 *         1: ok
 */
int init_lorawan_smartmodular_uart(void)
{
    int rc;
	int init_lorawan_smartmodular_uart_status = 0;

    LOG_INF("Starting LoRaWAN UART Smart Modular module...");

    rc = uart_configure(uart_lorawan, &uart_cfg);
    
    if (rc)
    {
        LOG_INF("Failed to configure lorawan uart");
		init_lorawan_smartmodular_uart_status = 0;
		goto END_INIT_LORAWAN_SMART_MODULAR;
	}
   
    /* Wake up lorawan module */
	wake_up_lorawan_smartmodular_uart();

	/* Reset lorawan module */
	reset_lorawan_smartmodular_uart();

	/* Configure join mode as ABP */
	join_mode_abp_smartmodular_uart();

	/* Configure LoRaWAN class as A */
	class_A_smartmodular_uart();

	/* Configure Device Address */
	device_address_smartmodular_uart();

	/* Configure Application Session Key */
    appskey_smartmodular_uart();

	/* Configure Network Session Key */
	nwskey_smartmodular_uart();

	/* Configure Application EUI */
	appeui_smartmodular_uart();

    LOG_INF("LoRaWAN UART Smart Modular module has been started");
    init_lorawan_smartmodular_uart_status = 1;

END_INIT_LORAWAN_SMART_MODULAR:	
	return init_lorawan_smartmodular_uart_status;
}

/* Function: send AT command to wake up lorawan module
 * Params: none
 * Return: none
 */
void wake_up_lorawan_smartmodular_uart(void)
{
    char at_cmd[100] = {0};

	memset(at_cmd, 0x00, sizeof(at_cmd));
	snprintf(at_cmd, sizeof(at_cmd), "%s%s", AT_CMD_WAKE, AT_CMD_TERMINATOR);
	send_custom_at_command_lorawan_smartmodular_uart(at_cmd);
	k_msleep(TIME_TO_RECEIVE_RESPONSE);
    rcv_response_lorawan_smartmodular_uart();
}

/* Function: send AT command to reset lorawan module
 * Params: none
 * Return: none
 */
void reset_lorawan_smartmodular_uart(void)
{
    char at_cmd[100] = {0};

	memset(at_cmd, 0x00, sizeof(at_cmd));
	snprintf(at_cmd, sizeof(at_cmd), "%s%s", AT_CMD_RESET, AT_CMD_TERMINATOR);
	send_custom_at_command_lorawan_smartmodular_uart(at_cmd);
	k_msleep(TIME_TO_RECEIVE_RESPONSE);
    rcv_response_lorawan_smartmodular_uart();
}

/* Function: send AT command to configure join mode as ABP
 * Params: none
 * Return: none
 */
void join_mode_abp_smartmodular_uart(void)
{
    char at_cmd[100] = {0};

	memset(at_cmd, 0x00, sizeof(at_cmd));
	snprintf(at_cmd, sizeof(at_cmd), "%s%s", AT_CMD_JOIN_MODE_ABP, AT_CMD_TERMINATOR);
	send_custom_at_command_lorawan_smartmodular_uart(at_cmd);
	k_msleep(TIME_TO_RECEIVE_RESPONSE);
    rcv_response_lorawan_smartmodular_uart();
}

/* Function: send AT command to configure lorawan class as A
 * Params: none
 * Return: none
 */
void class_A_smartmodular_uart(void)
{
	char at_cmd[100] = {0};

	memset(at_cmd, 0x00, sizeof(at_cmd));
	snprintf(at_cmd, sizeof(at_cmd), "%s%s", AT_CMD_CLASS, AT_CMD_TERMINATOR);
	send_custom_at_command_lorawan_smartmodular_uart(at_cmd);
	k_msleep(TIME_TO_RECEIVE_RESPONSE);
    rcv_response_lorawan_smartmodular_uart();
}

/* Function: send AT command to configure lorawan class as A
 * Params: none
 * Return: none
 */
void device_address_smartmodular_uart(void)
{
	char at_cmd[100] = {0};

	memset(at_cmd, 0x00, sizeof(at_cmd));
	snprintf(at_cmd, sizeof(at_cmd), "%s=%s%s", AT_CMD_DEVADDR, DEVADDR, AT_CMD_TERMINATOR);
	send_custom_at_command_lorawan_smartmodular_uart(at_cmd);
	k_msleep(TIME_TO_RECEIVE_RESPONSE);
    rcv_response_lorawan_smartmodular_uart();
}

/* Function: send AT command to configure lorawan application session key
 * Params: none
 * Return: none
 */
void appskey_smartmodular_uart(void)
{
    char at_cmd[100] = {0};

	memset(at_cmd, 0x00, sizeof(at_cmd));
	snprintf(at_cmd, sizeof(at_cmd), "%s=%s%s", AT_CMD_APPSKEY, APPSKEY, AT_CMD_TERMINATOR);
	send_custom_at_command_lorawan_smartmodular_uart(at_cmd);
	k_msleep(TIME_TO_RECEIVE_RESPONSE);
    rcv_response_lorawan_smartmodular_uart();
}

/* Function: send AT command to configure lorawan network session key
 * Params: none
 * Return: none
 */
void nwskey_smartmodular_uart(void)
{
    char at_cmd[100] = {0};

	memset(at_cmd, 0x00, sizeof(at_cmd));
	snprintf(at_cmd, sizeof(at_cmd), "%s=%s%s", AT_CMD_NWSKEY, NWKSKEY, AT_CMD_TERMINATOR);
	send_custom_at_command_lorawan_smartmodular_uart(at_cmd);
	k_msleep(TIME_TO_RECEIVE_RESPONSE);
    rcv_response_lorawan_smartmodular_uart();
}

/* Function: send AT command to configure lorawan application eui
 * Params: none
 * Return: none
 */
void appeui_smartmodular_uart(void)
{
    char at_cmd[100] = {0};

	memset(at_cmd, 0x00, sizeof(at_cmd));
	snprintf(at_cmd, sizeof(at_cmd), "%s=%s%s", AT_CMD_APPEUI, APPEUI, AT_CMD_TERMINATOR);
	send_custom_at_command_lorawan_smartmodular_uart(at_cmd);
	k_msleep(TIME_TO_RECEIVE_RESPONSE);
    rcv_response_lorawan_smartmodular_uart();
}

/* Function: send pulse counter as LoRaWAN message
 * Params: pulse counter
 * Return: none
 */
void send_pulse_counter(int total_pulses)
{
    char at_cmd[100] = {0};
	
	memset(at_cmd, 0x00, sizeof(at_cmd));
	snprintf(at_cmd, sizeof(at_cmd), "%s=%d:%08X%s", AT_CMD_SEND_BYTE, LORAWAN_PORT, total_pulses, AT_CMD_TERMINATOR);
	send_custom_at_command_lorawan_smartmodular_uart(at_cmd);
	k_msleep(TIME_TO_RECEIVE_RESPONSE);
    rcv_response_lorawan_smartmodular_uart();
}

/* Function: send custom AT command to smart modular lorawan module
 * Params: pointer to char array containing string
 * Return: none
 */
void send_custom_at_command_lorawan_smartmodular_uart(char *at_command)
{
	int msg_len = strlen(at_command);
    int i = 0;

	for (i=0; i<msg_len; i++) 
    {
		uart_poll_out(uart_lorawan, at_command[i]);
	}
}

/* Function: receive response from smart modular lorawan module
 * Params: 
 * Return: none
 */
void rcv_response_lorawan_smartmodular_uart(void)
{
    char c;
		
    LOG_INF("Bytes received from LoRaWAN module:");

	while (!uart_poll_in(uart_lorawan, &c)) 
	{
        LOG_INF("%c", c);
	}
}