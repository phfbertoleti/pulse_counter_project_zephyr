#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>
#include "breathing_light.h"
#include "prios.h"
#include "stacks_sizes.h"
#include "specific_debug_filters.h"


/* Specify which node is used for LED in device tree*/
#define LED0_NODE DT_ALIAS(led0)

/* Define time for toggling breathing light LED */
#define BREATHING_LIGHT_TIME_MS              1000

/* Define thread stack size */
K_THREAD_STACK_DEFINE(breathing_light_thread_stack, BREATHING_LIGHT_THREAD_STACKSIZE);

/* Load beathing light LED device tree data */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

/* Pulse counter thread data */
struct k_thread breathing_light_thread_data;

/* Prototype of breathing light task */
void breathing_light_task(void *p1, void *p2, void *p3);

/* Function: init breathing light, by setting up LED output
 * Params: none
 * Return: 0: error
 *         1: ok
 */
int init_breathing_light(void)
{
    int ret;

	if (!gpio_is_ready_dt(&led)) 
    {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) 
    {
		return 0;
	}

    /* Create pulse counter thread */
    k_thread_create(&breathing_light_thread_data, breathing_light_thread_stack,
			      K_THREAD_STACK_SIZEOF(breathing_light_thread_stack),
			      breathing_light_task, NULL, NULL, NULL,
			      BREATHING_LIGHT_THREAD_PRIORITY, 0, K_NO_WAIT);

    return 1;
}

/* Function: init breathing light, by setting up LED output
 * Params: none
 * Return: none
 */
void breathing_light_task(void *p1, void *p2, void *p3)
{
    while (1) 
    {
		gpio_pin_toggle_dt(&led);

        #ifdef ENABLE_DEBUG_MSG_BREATHING_LIGHT_LED_TOGGLING
            printk("[BREATHING LIGHT] Breathing light LED has been toggled\n");
        #endif

		k_msleep(BREATHING_LIGHT_TIME_MS);
	}
}