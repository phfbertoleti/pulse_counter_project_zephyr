/*
 * Pulse counter project
 * Author: Pedro Bertoleti
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/util.h>
#include <zephyr/logging/log.h>
#include "pulse_counter.h"
#include "breathing_light.h"
#include "lorawan_uart_smartmodular.h"

LOG_MODULE_REGISTER(pulse_counter_lorawan);

int main(void)
{
	int ret_init_lorawan = 0;
	int ret_init_breathing_light = 0;
	int ret_init_pulse_counter = 0;

	LOG_INF("Starting pulse counter project..."); 

    ret_init_breathing_light = init_breathing_light();
    ret_init_lorawan = init_lorawan_smartmodular_uart();	
    ret_init_pulse_counter = init_pulse_counter();	

	if ( (ret_init_breathing_light == 1) && (ret_init_lorawan == 1) && 
	     (ret_init_pulse_counter == 1) )
	{
        LOG_INF("All pulse counter project modules have been successfully started");
	}
	else
	{
		LOG_ERR("Some modules experienced errors in their init routines.");
	}

	return 0;
}
