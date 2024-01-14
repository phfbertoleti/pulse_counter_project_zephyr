#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>
#include <string.h>
#include <zephyr/logging/log.h>
#include "pulse_counter.h"
#include "lorawan_uart_smartmodular.h"

LOG_MODULE_DECLARE(pulse_counter_lorawan);

/*
 * Get pulse input configuration from the devicetree pulseinput alias.
 */
#define PULSEINPUT_NODE	DT_ALIAS(pulseinput)

/* Define time for toggling breathing light LED */
#define PULSE_COUNTER_TIME_MS              1000

/* Define time for main loop iteraction */
#define SLEEP_TIME_MS	60000

/* Define time used for debounce pulse input */
#define PULSE_DEBOUNCE_TIME    300  //ms

/* Pulse counter variable */
static int pulse_counter = 0;

/* Pulse debounce timer */
static struct k_timer pulse_debounce_timer;

/* Pulse counter thread data */
struct k_thread pulse_counter_thread_data;

#if !DT_NODE_HAS_STATUS(PULSEINPUT_NODE, okay)
#error "Unsupported board: pulseinput devicetree alias is not defined"
#endif
static const struct gpio_dt_spec pulseinput = GPIO_DT_SPEC_GET_OR(PULSEINPUT_NODE, gpios,
							      {0});
static struct gpio_callback pulseinput_cb_data;

/* Prototype of breathing light task */
void pulse_counter_task(void *p1, void *p2, void *p3);

/* Pulse debounce timer callback */
static void pulse_debounce_timer_callback(struct k_timer *timer);

/* Define thread stack */
K_THREAD_STACK_DEFINE(pulse_counter_thread_stack, CONFIG_PULSE_COUNTER_TASK_STACK_SIZE);

/* Function: pulse detection callback
 * Params: callback params
 * Return: none
 */
void pulse_detected(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	#if CONFIG_PULSE_COUNTER_VRBOSE_DEBUG_ENABLE
        LOG_INF("[PULSE_COUNTER] Pulse has been detected");
    #endif

	k_timer_start(&pulse_debounce_timer, K_MSEC(PULSE_DEBOUNCE_TIME), K_NO_WAIT);
}

/* Function: timer callback, used for debouncing pulses
 * Params: callback params
 * Return: none
 */
static void pulse_debounce_timer_callback(struct k_timer *timer)
{
	#if CONFIG_PULSE_COUNTER_VRBOSE_DEBUG_ENABLE
        LOG_INF("[PULSE_COUNTER] Pulse count has been updated!");
    #endif

	pulse_counter++;
}


/* Function: init pulse counter,  by setting up pulse counter input
 *           and pulse counter initial value
 * Params: none
 * Return: 0: error
 *         1: ok
 */
int init_pulse_counter(void)
{
    int ret;

    if (!gpio_is_ready_dt(&pulseinput)) 
	{
		#if CONFIG_PULSE_COUNTER_VRBOSE_DEBUG_ENABLE
            LOG_INF("[PULSE_COUNTER] Error: pulse input device %s is not ready", pulseinput.port->name);
        #endif

		return 0;
	}

	ret = gpio_pin_configure_dt(&pulseinput, GPIO_INPUT);
	if (ret != 0) 
	{
        #if CONFIG_PULSE_COUNTER_VRBOSE_DEBUG_ENABLE
		    LOG_INF("[PULSE_COUNTER] Error %d: failed to configure %s pin %d", ret, pulseinput.port->name, pulseinput.pin);
        #endif

		return 0;
	}

    /* Configure pulse interrupt edge as rising edge */
	ret = gpio_pin_interrupt_configure_dt(&pulseinput,
					      GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) 
	{
		#if CONFIG_PULSE_COUNTER_VRBOSE_DEBUG_ENABLE 
            LOG_INF("[PULSE_COUNTER] Error %d: failed to configure interrupt on %s pin %d", ret, pulseinput.port->name, pulseinput.pin);
        #endif

		return 0;
	}

	gpio_init_callback(&pulseinput_cb_data, pulse_detected, BIT(pulseinput.pin));
	gpio_add_callback(pulseinput.port, &pulseinput_cb_data);
	
    #if CONFIG_PULSE_COUNTER_VRBOSE_DEBUG_ENABLE
        LOG_INF("[PULSE_COUNTER] Set up pulse input at %s pin %d\n", pulseinput.port->name, pulseinput.pin);
    #endif

    /* Init pulse debounce timer */
    k_timer_init(&pulse_debounce_timer, pulse_debounce_timer_callback, NULL);

	/* Init pulse counter */
	pulse_counter = 0;

    #if CONFIG_PULSE_COUNTER_VRBOSE_DEBUG_ENABLE
	    LOG_INF("[PULSE_COUNTER] All set! Ready to read pulses");
    #endif

    /* Create pulse counter thread */
    k_thread_create(&pulse_counter_thread_data, pulse_counter_thread_stack,
			      K_THREAD_STACK_SIZEOF(pulse_counter_thread_stack),
			      pulse_counter_task, NULL, NULL, NULL,
			      CONFIG_PULSE_COUNTER_TASK_PRIO, 0, K_NO_WAIT);

    return 1;
}

/* Function: obtain pulse counter value
 * Params: none
 * Return: pulse counter value
 */
int get_pulse_counter(void)
{
    return pulse_counter;
}

/* Function: pulse counter task for monitoring pulses counter
 * Params: none
 * Return: none
 */
void pulse_counter_task(void *p1, void *p2, void *p3)
{
    while (1) 
	{
        /* Main loop */
		#if CONFIG_PULSE_COUNTER_VRBOSE_DEBUG_ENABLE
		    LOG_INF("[PULSE_COUNTER] Pulse counter value: %d", get_pulse_counter());
		#endif

        /* Send pulse counter as a LoRaWAN message */
        send_pulse_counter(pulse_counter);

        /* Thread cycle delay */
		k_msleep(SLEEP_TIME_MS);
	}
}