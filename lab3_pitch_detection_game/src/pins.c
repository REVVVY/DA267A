#include "pins.h"
#include "driver/gpio.h"

//LED PIN 12
#define LED_PIN 12


/* initialise the 12th pin */

void initPin()
{
    gpio_config_t cfg;
    cfg.pin_bit_mask = (u_int64_t)1 << LED_PIN; // initialize the 12th LED pin as output
    cfg.mode = GPIO_MODE_OUTPUT;
    cfg.pull_up_en = 0;
    cfg.pull_down_en = 0;
    gpio_config(&cfg);

    // you will need to use gpio_config()
}

/* switches LED on if level!=0 or off if level==0*/
void setLED(uint8_t level)
{
    if (level)
    {
        // set the pin of LED A to ON
        gpio_set_level(LED_PIN, 1); // you probably need to use gpio_set_level()
    }
    else
    {
        gpio_set_level(LED_PIN, 0); // set the pin of LED A to OFF
    }
}