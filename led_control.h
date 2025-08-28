#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include "gpio_mock.h"

// LED definitions
#define LED1_PIN GPIO_NUM_2
#define LED2_PIN GPIO_NUM_4
#define LED3_PIN GPIO_NUM_5

#define NUM_LEDS 3

// LED states
typedef enum {
    LED_OFF = 0,
    LED_ON = 1
} led_state_t;

// LED structure
typedef struct {
    uint32_t pin;
    led_state_t state;
    const char* name;
} led_t;

// Function declarations
void led_init_all(void);
void led_set_state(uint32_t led_pin, led_state_t state);
void led_toggle(uint32_t led_pin);
void led_turn_on(uint32_t led_pin);
void led_turn_off(uint32_t led_pin);
led_state_t led_get_state(uint32_t led_pin);
void led_all_off(void);
void led_all_on(void);
void led_display_status(void);
const char* led_get_name(uint32_t led_pin);

#endif // LED_CONTROL_H