
/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// Fade an LED between low and high brightness. An interrupt handler updates
// the PWM slice's output level each time the counter wraps.

#include "pico/stdlib.h"
#include <stdio.h>
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include <stdlib.h>
#include <unistd.h>

// variable to count the seconds.
int count = 1;

#ifdef PICO_DEFAULT_LED_PIN
void on_pwm_wrap() {
    static int fade = 0;
    static bool going_up = true;
    
    // Clear the interrupt flag that brought us here (repeated for pins 0,1,2,3)
    pwm_clear_irq(pwm_gpio_to_slice_num(PICO_DEFAULT_LED_PIN));
    pwm_clear_irq(pwm_gpio_to_slice_num(0));
    pwm_clear_irq(pwm_gpio_to_slice_num(1));
    pwm_clear_irq(pwm_gpio_to_slice_num(2));
    pwm_clear_irq(pwm_gpio_to_slice_num(3));

    if (going_up) {
        fade ++;
        if (fade == 250) going_up = false;
        
    } else {
        fade --;
        if (fade == 0) {
            going_up = true;
            count += 1;
        }
    }
    // Square the fade value to make the LED's brightness appear more linear
    // Note this range matches with the wrap value (repeated for pins 0,1,2,3)
    pwm_set_gpio_level(PICO_DEFAULT_LED_PIN, fade * fade);
    pwm_set_gpio_level(0, fade * fade * ((count>>0)%2));
    pwm_set_gpio_level(1, fade * fade * ((count>>1)%2));
    pwm_set_gpio_level(2, fade * fade * ((count>>2)%2));
    pwm_set_gpio_level(3, fade * fade * ((count>>3)%2));
    //https://raspberrypi.github.io/pico-sdk-doxygen/group__hardware__pwm.html#ga279d1ba7dcc8f19619f389317efb41fd
}

#endif

int main(void) {
#ifndef PICO_DEFAULT_LED_PIN
#error pwm/led_fade example requires a board with a regular LED
#else
    // Tell the LED pin that the PWM is in charge of its value.(repeated for pins 0,1,2,3)
    gpio_set_function(PICO_DEFAULT_LED_PIN, GPIO_FUNC_PWM);
    gpio_set_function(0, GPIO_FUNC_PWM);
    gpio_set_function(1, GPIO_FUNC_PWM);
    gpio_set_function(2, GPIO_FUNC_PWM);
    gpio_set_function(3, GPIO_FUNC_PWM);

    // Figure out which slice we just connected to the LED pin(repeated for pins 0,1,2,3)
    uint slice_num25 = pwm_gpio_to_slice_num(PICO_DEFAULT_LED_PIN);
    uint slice_num0 = pwm_gpio_to_slice_num(0);
    uint slice_num1 = pwm_gpio_to_slice_num(1);
    uint slice_num2 = pwm_gpio_to_slice_num(2);
    uint slice_num3 = pwm_gpio_to_slice_num(3);

    // Mask our slice's IRQ output into the PWM block's single interrupt line,(repeated for pins 0,1,2,3)
    // and register our interrupt handler
    pwm_clear_irq(slice_num25);
    pwm_set_irq_enabled(slice_num25, true);
    pwm_clear_irq(slice_num0);
    pwm_set_irq_enabled(slice_num0, true);    
    pwm_clear_irq(slice_num1);
    pwm_set_irq_enabled(slice_num1, true); 
    pwm_clear_irq(slice_num2);
    pwm_set_irq_enabled(slice_num2, true); 
    pwm_clear_irq(slice_num3);
    pwm_set_irq_enabled(slice_num3, true); 
    irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap);
    irq_set_enabled(PWM_IRQ_WRAP, true);

    // Get some sensible defaults for the slice configuration. By default, the
    // counter is allowed to wrap over its maximum range (0 to 2**16-1)
    pwm_config config = pwm_get_default_config();
    // Set divider, reduces counter clock to sysclock/this value
    pwm_config_set_clkdiv(&config, 12);
    // Load the configuration into our PWM slice, and set it running.(repeated for pins 0,1,2,3)
    pwm_init(slice_num25, &config, true);
    pwm_init(slice_num0, &config, true);
    pwm_init(slice_num1, &config, true);
    pwm_init(slice_num2, &config, true);
    pwm_init(slice_num3, &config, true);

    // Everything after this point happens in the PWM interrupt handler, so we
    // can twiddle our thumbs
    while (true)
        tight_loop_contents();
#endif
}
