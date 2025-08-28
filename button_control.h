#ifndef BUTTON_CONTROL_H
#define BUTTON_CONTROL_H

#include "gpio_mock.h"
#include <stdbool.h>

// Button definitions
#define BUTTON1_PIN GPIO_NUM_18
#define BUTTON2_PIN GPIO_NUM_19
#define BUTTON3_PIN GPIO_NUM_21

#define NUM_BUTTONS 3
#define DEBOUNCE_DELAY_MS 50

// Button states
typedef enum {
    BUTTON_RELEASED = 0,
    BUTTON_PRESSED = 1
} button_state_t;

// Button structure for debouncing
typedef struct {
    uint32_t pin;
    button_state_t current_state;
    button_state_t last_state;
    uint32_t last_debounce_time;
    bool state_changed;
    const char* name;
} button_t;

// Function declarations
void button_init_all(void);
void button_update_all(void);
button_state_t button_get_state(uint32_t button_pin);
bool button_is_pressed(uint32_t button_pin);
bool button_was_pressed(uint32_t button_pin);
bool button_was_released(uint32_t button_pin);
void button_clear_events(uint32_t button_pin);
void button_display_status(void);
const char* button_get_name(uint32_t button_pin);
uint32_t button_get_time_ms(void);

// Simulation functions (for testing)
void button_simulate_press(uint32_t button_pin);
void button_simulate_release(uint32_t button_pin);

#endif // BUTTON_CONTROL_H