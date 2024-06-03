/**
 * Copyright (c) 2024 Wayne Bagguley
 *
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

static const uint BUTTON_MOTOR_PIN = 19;
static const uint BUTTON_LED_PIN = 20;
static const uint BUTTON_LASER_PIN = 21;
static const uint MOTOR_PIN = 12;
static const uint LED_PIN = 11;
static const uint LASER_PIN = 10;

static const uint64_t DEBOUNCE_TIME = 20000;

static volatile bool led_state = false;
static volatile bool led_button_state = false;
static volatile uint64_t led_time = 0;

static volatile bool motor_state = false;
static volatile bool motor_button_state = false;
static volatile uint64_t motor_time = 0;

static volatile bool laser_state = false;
static volatile bool laser_button_state = false;
static volatile uint64_t laser_time = 0;

void irq_callback(uint gpio, uint32_t event_mask) {
    switch (gpio)
    {
        case BUTTON_LED_PIN: {
                uint64_t current_time = time_us_64();
                if (current_time > led_time + DEBOUNCE_TIME) {
                    led_time = current_time;
                    if (led_button_state == false && event_mask & GPIO_IRQ_EDGE_FALL) {
                        led_state = !led_state;
                        led_button_state = true;
                    }
                    if (led_button_state == true && event_mask & GPIO_IRQ_EDGE_RISE) {
                        led_button_state = false;
                    }
                }
            }
            break;
        case BUTTON_MOTOR_PIN: {
                uint64_t current_time = time_us_64();
                if (current_time > motor_time + DEBOUNCE_TIME) {
                    motor_time = current_time;
                    if (motor_button_state == false && event_mask & GPIO_IRQ_EDGE_FALL) {
                        motor_state = !motor_state;
                        motor_button_state = true;
                    }
                    if (motor_button_state == true && event_mask & GPIO_IRQ_EDGE_RISE) {
                        motor_button_state = false;
                    }
                }
            }
            break;
        case BUTTON_LASER_PIN: {
                uint64_t current_time = time_us_64();
                if (current_time > laser_time + DEBOUNCE_TIME) {
                    laser_time = current_time;
                    if (laser_button_state == false && event_mask & GPIO_IRQ_EDGE_FALL) {
                        laser_state = !laser_state;
                        laser_button_state = true;
                    }
                    if (laser_button_state == true && event_mask & GPIO_IRQ_EDGE_RISE) {
                        laser_button_state = false;
                    }
                }
            }
            break;
        default:
            break;
    }
}

int main() {
    // Init the LED button pin, set to input and pull-up
    gpio_init(BUTTON_LED_PIN);
    gpio_set_dir(BUTTON_LED_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_LED_PIN);
    
    // Init the LED pin, set to output
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Enable IRQ for the LED button
    gpio_set_irq_enabled(BUTTON_LED_PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true);

    // Init the motor button pin, set to input and pull-up
    gpio_init(BUTTON_MOTOR_PIN);
    gpio_set_dir(BUTTON_MOTOR_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_MOTOR_PIN);

    // Init the motor pin, set to output
    gpio_init(MOTOR_PIN);
    gpio_set_dir(MOTOR_PIN, GPIO_OUT);

    // Enable IRQ for the motor button
    gpio_set_irq_enabled(BUTTON_MOTOR_PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true);

    // Init the laser button pin, set to input and pull-up
    gpio_init(BUTTON_LASER_PIN);
    gpio_set_dir(BUTTON_LASER_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_LASER_PIN);

    // Init the laser pin, set to output
    gpio_init(LASER_PIN);
    gpio_set_dir(LASER_PIN, GPIO_OUT);

    // Enable IRQ for the laser button
    gpio_set_irq_enabled(BUTTON_LASER_PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true);

    // Set the IRQ function
    gpio_set_irq_callback(&irq_callback);

    // Enable IRQ
    irq_set_enabled(IO_IRQ_BANK0, true);

    // Loop forever
    while (1) {
        sleep_ms(100);
        if (led_state) {
            gpio_put(LED_PIN, 1);
        } else {
            gpio_put(LED_PIN, 0);
        }
        if (motor_state) {
            gpio_put(MOTOR_PIN, 1);
        } else {
            gpio_put(MOTOR_PIN, 0);
        }
        if (laser_state) {
            gpio_put(LASER_PIN, 1);
        } else {
            gpio_put(LASER_PIN, 0);
        }
    }
}