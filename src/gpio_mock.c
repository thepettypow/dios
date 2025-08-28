#include "gpio_mock.h"
#include <stdio.h>
#include <string.h>

// Mock GPIO register simulation
#define MAX_GPIO_PINS 40

// Simulated GPIO registers
static struct {
    uint32_t output_level[MAX_GPIO_PINS];  // Output level register
    uint32_t input_level[MAX_GPIO_PINS];   // Input level register
    gpio_mode_t mode[MAX_GPIO_PINS];       // Pin mode register
    gpio_pullup_t pullup[MAX_GPIO_PINS];   // Pull-up configuration
    bool initialized[MAX_GPIO_PINS];       // Track initialized pins
} gpio_registers;

// Initialize the GPIO mock system
void gpio_mock_init(void) {
    // Clear all registers
    memset(&gpio_registers, 0, sizeof(gpio_registers));
    
    // Set default button states (simulate buttons not pressed)
    gpio_registers.input_level[GPIO_NUM_18] = GPIO_LEVEL_HIGH;  // Button 1
    gpio_registers.input_level[GPIO_NUM_19] = GPIO_LEVEL_HIGH;  // Button 2
    gpio_registers.input_level[GPIO_NUM_21] = GPIO_LEVEL_HIGH;  // Button 3
    
    printf("[GPIO] Mock GPIO system initialized\n");
}

// Configure GPIO pin
void gpio_config_pin(gpio_config_t *gpio_conf) {
    if (!gpio_conf) {
        printf("[GPIO ERROR] Invalid configuration pointer\n");
        return;
    }
    
    // Configure each pin in the bit mask
    for (int pin = 0; pin < MAX_GPIO_PINS; pin++) {
        if (gpio_conf->pin_bit_mask & (1ULL << pin)) {
            if (!GPIO_IS_VALID_GPIO(pin)) {
                printf("[GPIO ERROR] Invalid GPIO pin: %d\n", pin);
                continue;
            }
            
            gpio_registers.mode[pin] = gpio_conf->mode;
            gpio_registers.pullup[pin] = gpio_conf->pull_up_en;
            gpio_registers.initialized[pin] = true;
            
            // Initialize output pins to LOW
            if (gpio_conf->mode == GPIO_MODE_OUTPUT) {
                gpio_registers.output_level[pin] = GPIO_LEVEL_LOW;
            }
            
            printf("[GPIO] Pin %d configured as %s\n", 
                   pin, 
                   (gpio_conf->mode == GPIO_MODE_OUTPUT) ? "OUTPUT" : "INPUT");
        }
    }
}

// Set GPIO output level
void gpio_set_level(uint32_t gpio_num, uint32_t level) {
    if (!GPIO_IS_VALID_GPIO(gpio_num)) {
        printf("[GPIO ERROR] Invalid GPIO pin: %d\n", gpio_num);
        return;
    }
    
    if (!gpio_registers.initialized[gpio_num]) {
        printf("[GPIO ERROR] GPIO pin %d not initialized\n", gpio_num);
        return;
    }
    
    if (gpio_registers.mode[gpio_num] != GPIO_MODE_OUTPUT) {
        printf("[GPIO ERROR] GPIO pin %d not configured as output\n", gpio_num);
        return;
    }
    
    gpio_registers.output_level[gpio_num] = (level != 0) ? GPIO_LEVEL_HIGH : GPIO_LEVEL_LOW;
    
    printf("[GPIO] Pin %d set to %s\n", 
           gpio_num, 
           (gpio_registers.output_level[gpio_num] == GPIO_LEVEL_HIGH) ? "HIGH" : "LOW");
}

// Get GPIO input level
uint32_t gpio_get_level(uint32_t gpio_num) {
    if (!GPIO_IS_VALID_GPIO(gpio_num)) {
        printf("[GPIO ERROR] Invalid GPIO pin: %d\n", gpio_num);
        return 0;
    }
    
    if (!gpio_registers.initialized[gpio_num]) {
        printf("[GPIO ERROR] GPIO pin %d not initialized\n", gpio_num);
        return 0;
    }
    
    if (gpio_registers.mode[gpio_num] == GPIO_MODE_INPUT) {
        return gpio_registers.input_level[gpio_num];
    } else {
        // For output pins, return the output level
        return gpio_registers.output_level[gpio_num];
    }
}

// Toggle GPIO output level
void gpio_toggle_level(uint32_t gpio_num) {
    if (!GPIO_IS_VALID_GPIO(gpio_num)) {
        printf("[GPIO ERROR] Invalid GPIO pin: %d\n", gpio_num);
        return;
    }
    
    if (!gpio_registers.initialized[gpio_num]) {
        printf("[GPIO ERROR] GPIO pin %d not initialized\n", gpio_num);
        return;
    }
    
    if (gpio_registers.mode[gpio_num] != GPIO_MODE_OUTPUT) {
        printf("[GPIO ERROR] GPIO pin %d not configured as output\n", gpio_num);
        return;
    }
    
    gpio_registers.output_level[gpio_num] = 
        (gpio_registers.output_level[gpio_num] == GPIO_LEVEL_HIGH) ? 
        GPIO_LEVEL_LOW : GPIO_LEVEL_HIGH;
    
    printf("[GPIO] Pin %d toggled to %s\n", 
           gpio_num, 
           (gpio_registers.output_level[gpio_num] == GPIO_LEVEL_HIGH) ? "HIGH" : "LOW");
}

// Print current GPIO status (for debugging)
void gpio_print_status(void) {
    printf("\n=== GPIO Status ===\n");
    printf("LEDs:\n");
    printf("  LED1 (Pin %d): %s\n", GPIO_NUM_2, 
           gpio_registers.initialized[GPIO_NUM_2] ? 
           (gpio_registers.output_level[GPIO_NUM_2] ? "ON" : "OFF") : "NOT_INIT");
    printf("  LED2 (Pin %d): %s\n", GPIO_NUM_4, 
           gpio_registers.initialized[GPIO_NUM_4] ? 
           (gpio_registers.output_level[GPIO_NUM_4] ? "ON" : "OFF") : "NOT_INIT");
    printf("  LED3 (Pin %d): %s\n", GPIO_NUM_5, 
           gpio_registers.initialized[GPIO_NUM_5] ? 
           (gpio_registers.output_level[GPIO_NUM_5] ? "ON" : "OFF") : "NOT_INIT");
    
    printf("Buttons:\n");
    printf("  BTN1 (Pin %d): %s\n", GPIO_NUM_18, 
           gpio_registers.initialized[GPIO_NUM_18] ? 
           (gpio_registers.input_level[GPIO_NUM_18] ? "RELEASED" : "PRESSED") : "NOT_INIT");
    printf("  BTN2 (Pin %d): %s\n", GPIO_NUM_19, 
           gpio_registers.initialized[GPIO_NUM_19] ? 
           (gpio_registers.input_level[GPIO_NUM_19] ? "RELEASED" : "PRESSED") : "NOT_INIT");
    printf("  BTN3 (Pin %d): %s\n", GPIO_NUM_21, 
           gpio_registers.initialized[GPIO_NUM_21] ? 
           (gpio_registers.input_level[GPIO_NUM_21] ? "RELEASED" : "PRESSED") : "NOT_INIT");
    printf("==================\n\n");
}

// Simulate button press (for testing purposes)
void gpio_simulate_button_press(uint32_t gpio_num) {
    if (gpio_num == GPIO_NUM_18 || gpio_num == GPIO_NUM_19 || gpio_num == GPIO_NUM_21) {
        gpio_registers.input_level[gpio_num] = GPIO_LEVEL_LOW;
        printf("[SIMULATION] Button on pin %d pressed\n", gpio_num);
    }
}

// Simulate button release (for testing purposes)
void gpio_simulate_button_release(uint32_t gpio_num) {
    if (gpio_num == GPIO_NUM_18 || gpio_num == GPIO_NUM_19 || gpio_num == GPIO_NUM_21) {
        gpio_registers.input_level[gpio_num] = GPIO_LEVEL_HIGH;
        printf("[SIMULATION] Button on pin %d released\n", gpio_num);
    }
}