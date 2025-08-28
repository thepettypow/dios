#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include "gpio_mock.h"
#include "led_control.h"
#include "button_control.h"

// Global flag for graceful shutdown
static volatile bool running = true;

// Signal handler for graceful shutdown
void signal_handler(int sig) {
    printf("\n[MAIN] Received signal %d, shutting down...\n", sig);
    running = false;
}

// Display help menu
void display_help(void) {
    printf("\n=== ESP32 LED Control Simulation ===\n");
    printf("Commands:\n");
    printf("  1, 2, 3    - Simulate button press on BTN1, BTN2, BTN3\n");
    printf("  r1, r2, r3 - Simulate button release on BTN1, BTN2, BTN3\n");
    printf("  s          - Show status of all LEDs and buttons\n");
    printf("  h          - Show this help menu\n");
    printf("  q          - Quit program\n");
    printf("\nButton-LED mapping:\n");
    printf("  BTN1 (Pin 18) -> LED1 (Pin 2)\n");
    printf("  BTN2 (Pin 19) -> LED2 (Pin 4)\n");
    printf("  BTN3 (Pin 21) -> LED3 (Pin 5)\n");
    printf("=====================================\n\n");
}

// Process button events and control LEDs
void process_button_events(void) {
    // Check each button for press events
    if (button_was_pressed(BUTTON1_PIN)) {
        printf("[MAIN] Button 1 pressed - Toggling LED1\n");
        led_toggle(LED1_PIN);
    }
    
    if (button_was_pressed(BUTTON2_PIN)) {
        printf("[MAIN] Button 2 pressed - Toggling LED2\n");
        led_toggle(LED2_PIN);
    }
    
    if (button_was_pressed(BUTTON3_PIN)) {
        printf("[MAIN] Button 3 pressed - Toggling LED3\n");
        led_toggle(LED3_PIN);
    }
}

// Handle user input for simulation
void handle_user_input(void) {
    // Check if input is available (non-blocking)
    fd_set readfds;
    struct timeval timeout;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    
    if (select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout) > 0) {
        char input[10];
        if (fgets(input, sizeof(input), stdin) != NULL) {
            switch (input[0]) {
                case '1':
                    button_simulate_press(BUTTON1_PIN);
                    break;
                case '2':
                    button_simulate_press(BUTTON2_PIN);
                    break;
                case '3':
                    button_simulate_press(BUTTON3_PIN);
                    break;
                case 'r':
                    if (input[1] == '1') {
                        button_simulate_release(BUTTON1_PIN);
                    } else if (input[1] == '2') {
                        button_simulate_release(BUTTON2_PIN);
                    } else if (input[1] == '3') {
                        button_simulate_release(BUTTON3_PIN);
                    }
                    break;
                case 's':
                    led_display_status();
                    button_display_status();
                    break;
                case 'h':
                    display_help();
                    break;
                case 'q':
                    running = false;
                    break;
                default:
                    printf("[MAIN] Unknown command. Type 'h' for help.\n");
                    break;
            }
        }
    }
}

// Initialize all systems
void system_init(void) {
    printf("[MAIN] Starting ESP32 LED Control Simulation...\n");
    
    // Initialize GPIO mock system
    gpio_mock_init();
    
    // Initialize LEDs
    led_init_all();
    
    // Initialize buttons
    button_init_all();
    
    printf("[MAIN] System initialization complete!\n\n");
}

// Main application loop
void app_loop(void) {
    printf("[MAIN] Entering main loop. Type 'h' for help, 'q' to quit.\n\n");
    
    while (running) {
        // Update button states
        button_update_all();
        
        // Process button events and control LEDs
        process_button_events();
        
        // Handle user input for simulation
        handle_user_input();
        
        // Small delay to prevent excessive CPU usage
        usleep(10000); // 10ms delay
    }
}

// Cleanup and shutdown
void system_shutdown(void) {
    printf("\n[MAIN] Shutting down system...\n");
    
    // Turn off all LEDs
    led_all_off();
    
    // Display final status
    printf("\n=== Final System Status ===\n");
    led_display_status();
    button_display_status();
    
    printf("[MAIN] System shutdown complete. Goodbye!\n");
}

int main(void) {
    // Set up signal handlers for graceful shutdown
    signal(SIGINT, signal_handler);   // Ctrl+C
    signal(SIGTERM, signal_handler);  // Termination signal
    
    // Initialize system
    system_init();
    
    // Show help menu
    display_help();
    
    // Run main application loop
    app_loop();
    
    // Cleanup and shutdown
    system_shutdown();
    
    return 0;
}