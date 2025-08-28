#include "button_control.h"
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

// Button array for easy management
static button_t buttons[NUM_BUTTONS] = {
    {BUTTON1_PIN, BUTTON_RELEASED, BUTTON_RELEASED, 0, false, "BTN1"},
    {BUTTON2_PIN, BUTTON_RELEASED, BUTTON_RELEASED, 0, false, "BTN2"},
    {BUTTON3_PIN, BUTTON_RELEASED, BUTTON_RELEASED, 0, false, "BTN3"}
};

// Get current time in milliseconds
uint32_t button_get_time_ms(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint32_t)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

// Initialize all buttons
void button_init_all(void) {
    printf("[BUTTON] Initializing buttons...\n");
    
    // Configure button pins as inputs with pull-up
    gpio_config_t button_config = {
        .pin_bit_mask = GPIO_PIN_SEL(BUTTON1_PIN) | GPIO_PIN_SEL(BUTTON2_PIN) | GPIO_PIN_SEL(BUTTON3_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE
    };
    
    gpio_config_pin(&button_config);
    
    // Initialize button states
    uint32_t current_time = button_get_time_ms();
    for (int i = 0; i < NUM_BUTTONS; i++) {
        buttons[i].current_state = BUTTON_RELEASED;
        buttons[i].last_state = BUTTON_RELEASED;
        buttons[i].last_debounce_time = current_time;
        buttons[i].state_changed = false;
    }
    
    printf("[BUTTON] All buttons initialized\n");
}

// Update all button states (should be called regularly)
void button_update_all(void) {
    uint32_t current_time = button_get_time_ms();
    
    for (int i = 0; i < NUM_BUTTONS; i++) {
        // Read raw button state (inverted because of pull-up)
        uint32_t raw_state = gpio_get_level(buttons[i].pin);
        button_state_t new_state = (raw_state == GPIO_LEVEL_LOW) ? BUTTON_PRESSED : BUTTON_RELEASED;
        
        // Reset state change flag
        buttons[i].state_changed = false;
        
        // Check if state has changed
        if (new_state != buttons[i].last_state) {
            buttons[i].last_debounce_time = current_time;
        }
        
        // Check if enough time has passed for debouncing
        if ((current_time - buttons[i].last_debounce_time) > DEBOUNCE_DELAY_MS) {
            // If the state has changed after debounce period
            if (new_state != buttons[i].current_state) {
                buttons[i].current_state = new_state;
                buttons[i].state_changed = true;
                
                printf("[BUTTON] %s %s\n", 
                       buttons[i].name, 
                       (new_state == BUTTON_PRESSED) ? "PRESSED" : "RELEASED");
            }
        }
        
        buttons[i].last_state = new_state;
    }
}

// Get button state
button_state_t button_get_state(uint32_t button_pin) {
    for (int i = 0; i < NUM_BUTTONS; i++) {
        if (buttons[i].pin == button_pin) {
            return buttons[i].current_state;
        }
    }
    printf("[BUTTON ERROR] Invalid button pin: %d\n", button_pin);
    return BUTTON_RELEASED;
}

// Check if button is currently pressed
bool button_is_pressed(uint32_t button_pin) {
    return button_get_state(button_pin) == BUTTON_PRESSED;
}

// Check if button was just pressed (edge detection)
bool button_was_pressed(uint32_t button_pin) {
    for (int i = 0; i < NUM_BUTTONS; i++) {
        if (buttons[i].pin == button_pin) {
            return (buttons[i].state_changed && buttons[i].current_state == BUTTON_PRESSED);
        }
    }
    return false;
}

// Check if button was just released (edge detection)
bool button_was_released(uint32_t button_pin) {
    for (int i = 0; i < NUM_BUTTONS; i++) {
        if (buttons[i].pin == button_pin) {
            return (buttons[i].state_changed && buttons[i].current_state == BUTTON_RELEASED);
        }
    }
    return false;
}

// Clear button events (reset state_changed flag)
void button_clear_events(uint32_t button_pin) {
    for (int i = 0; i < NUM_BUTTONS; i++) {
        if (buttons[i].pin == button_pin) {
            buttons[i].state_changed = false;
            return;
        }
    }
}

// Display button status
void button_display_status(void) {
    printf("\n=== Button Status ===\n");
    for (int i = 0; i < NUM_BUTTONS; i++) {
        printf("  %s (Pin %d): %s\n", 
               buttons[i].name, 
               buttons[i].pin, 
               (buttons[i].current_state == BUTTON_PRESSED) ? "PRESSED" : "RELEASED");
    }
    printf("=====================\n\n");
}

// Get button name
const char* button_get_name(uint32_t button_pin) {
    for (int i = 0; i < NUM_BUTTONS; i++) {
        if (buttons[i].pin == button_pin) {
            return buttons[i].name;
        }
    }
    return "UNKNOWN";
}

// Simulation functions for testing
void button_simulate_press(uint32_t button_pin) {
    // Use the GPIO mock simulation functions
    extern void gpio_simulate_button_press(uint32_t gpio_num);
    gpio_simulate_button_press(button_pin);
}

void button_simulate_release(uint32_t button_pin) {
    // Use the GPIO mock simulation functions
    extern void gpio_simulate_button_release(uint32_t gpio_num);
    gpio_simulate_button_release(button_pin);
}