/*
 * Pulse counter project
 * Author: Pedro Bertoleti
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>
#include <zephyr/sys/__assert.h>
#include "pulse_counter.h"
#include "breathing_light.h"
#include "lorawan_uart_smartmodular.h"
#include "specific_debug_filters.h"

int main(void)
{
	int status_init_lorawan;

	/* Init breathing light */
	printk("Init: breathing light\r\n");
	init_breathing_light();

    /* Init LoRaWAN uart */
	printk("Init: LoRaWAN module: smart modular\r\n");
	status_init_lorawan = init_lorawan_smartmodular_uart();	

	/* Init pulse counter */
	if (status_init_lorawan == 0)
	{
		printk("Init: pulse counter\r\n");
	    init_pulse_counter();	
	}

	return 0;
}
