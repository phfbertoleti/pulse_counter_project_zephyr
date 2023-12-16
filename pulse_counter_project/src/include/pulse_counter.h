/* Header file: pulse counter */

#ifndef HEADER_PULSE_COUNTER
#define HEADER_PULSE_COUNTER

#endif

/* Prototypes */
void pulse_detected(const struct device *dev, struct gpio_callback *cb, uint32_t pins);
int init_pulse_counter(void);
int get_pulse_counter(void);