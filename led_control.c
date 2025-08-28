#include "led_control.h"
#include <stdio.h>

// LED array for easy management
static led_t leds[NUM_LEDS] = {
    {LED1_PIN, LED_OFF, "LED1"},
    {LED2_PIN, LED_OFF, "LED2"},
    {LED3_PIN, LED_OFF, "LED3"}
};

// Initialize all LEDs
void led_init_all(void) {
    printf("[LED] Initializing LEDs...\n");
    
    // Configure LED pins as outputs
    gpio_config_t led_config = {
        .pin_bit_mask = GPIO_PIN_SEL(LED1_PIN) | GPIO_PIN_SEL(LED2_PIN) | GPIO_PIN_SEL(LED3_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE
    };
    
    gpio_config_pin(&led_config);
    
    // Turn off all LEDs initially
    led_all_off();
    
    printf("[LED] All LEDs initialized and turned OFF\n");
}

// Set LED state
void led_set_state(uint32_t led_pin, led_state_t state) {
    // Find the LED in our array
    for (int i = 0; i < NUM_LEDS; i++) {
        if (leds[i].pin == led_pin) {
            leds[i].state = state;
            gpio_set_level(led_pin, (state == LED_ON) ? GPIO_LEVEL_HIGH : GPIO_LEVEL_LOW);
            printf("[LED] %s turned %s\n", leds[i].name, (state == LED_ON) ? "ON" : "OFF");
            return;
        }
    }
    printf("[LED ERROR] Invalid LED pin: %d\n", led_pin);
}

// Toggle LED state
void led_toggle(uint32_t led_pin) {
    for (int i = 0; i < NUM_LEDS; i++) {
        if (leds[i].pin == led_pin) {
            led_state_t new_state = (leds[i].state == LED_ON) ? LED_OFF : LED_ON;
            led_set_state(led_pin, new_state);
            return;
        }
    }
    printf("[LED ERROR] Invalid LED pin for toggle: %d\n", led_pin);
}

// Turn LED on
void led_turn_on(uint32_t led_pin) {
    led_set_state(led_pin, LED_ON);
}

// Turn LED off
void led_turn_off(uint32_t led_pin) {
    led_set_state(led_pin, LED_OFF);
}

// Get LED state
led_state_t led_get_state(uint32_t led_pin) {
    for (int i = 0; i < NUM_LEDS; i++) {
        if (leds[i].pin == led_pin) {
            return leds[i].state;
        }
    }
    printf("[LED ERROR] Invalid LED pin for state query: %d\n", led_pin);
    return LED_OFF;
}

// Turn all LEDs off
void led_all_off(void) {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i].state = LED_OFF;
        gpio_set_level(leds[i].pin, GPIO_LEVEL_LOW);
    }
    printf("[LED] All LEDs turned OFF\n");
}

// Turn all LEDs on
void led_all_on(void) {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i].state = LED_ON;
        gpio_set_level(leds[i].pin, GPIO_LEVEL_HIGH);
    }
    printf("[LED] All LEDs turned ON\n");
}

// Display LED status
void led_display_status(void) {
    printf("\n=== LED Status ===\n");
    for (int i = 0; i < NUM_LEDS; i++) {
        printf("  %s (Pin %d): %s\n", 
               leds[i].name, 
               leds[i].pin, 
               (leds[i].state == LED_ON) ? "ON" : "OFF");
    }
    printf("==================\n\n");
}

// Get LED name
const char* led_get_name(uint32_t led_pin) {
    for (int i = 0; i < NUM_LEDS; i++) {
        if (leds[i].pin == led_pin) {
            return leds[i].name;
        }
    }
    return "UNKNOWN";
}