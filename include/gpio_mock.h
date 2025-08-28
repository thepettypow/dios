#ifndef GPIO_MOCK_H
#define GPIO_MOCK_H

#include <stdint.h>
#include <stdbool.h>

// GPIO pin definitions (simulating ESP32 pins)
#define GPIO_NUM_2   2   // LED 1
#define GPIO_NUM_4   4   // LED 2
#define GPIO_NUM_5   5   // LED 3
#define GPIO_NUM_18  18  // Button 1
#define GPIO_NUM_19  19  // Button 2
#define GPIO_NUM_21  21  // Button 3

// GPIO modes
typedef enum {
    GPIO_MODE_INPUT = 0,
    GPIO_MODE_OUTPUT = 1
} gpio_mode_t;

// GPIO pull modes
typedef enum {
    GPIO_PULLUP_DISABLE = 0,
    GPIO_PULLUP_ENABLE = 1
} gpio_pullup_t;

// GPIO levels
typedef enum {
    GPIO_LEVEL_LOW = 0,
    GPIO_LEVEL_HIGH = 1
} gpio_level_t;

// GPIO configuration structure
typedef struct {
    uint32_t pin_bit_mask;     // GPIO pin: set with bit mask
    gpio_mode_t mode;          // GPIO mode: set input/output mode
    gpio_pullup_t pull_up_en;  // GPIO pull-up
} gpio_config_t;

// Function declarations
void gpio_mock_init(void);
void gpio_config_pin(gpio_config_t *gpio_conf);
void gpio_set_level(uint32_t gpio_num, uint32_t level);
uint32_t gpio_get_level(uint32_t gpio_num);
void gpio_toggle_level(uint32_t gpio_num);
void gpio_print_status(void);

// Helper macros
#define GPIO_PIN_SEL(pin) (1ULL << (pin))
#define GPIO_IS_VALID_GPIO(gpio_num) ((gpio_num) < 40)

#endif // GPIO_MOCK_H