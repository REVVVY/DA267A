#include "pins.h"
#include "driver/gpio.h"

//LED PINS
#define LED_PIN_RED 12
#define LED_PIN_BLUE 14

//BUTTON PINS
#define BUTTON_PIN_RED 26
#define BUTTON_PIN_YELLOW 27

/* initialises the 4 pins */

void initPins()
{
    gpio_config_t cfg;
    cfg.pin_bit_mask = (u_int64_t)1 << LED_PIN_RED; // initialize 2 LEDs pins as output and the 2 buttons' pins as input
    cfg.pin_bit_mask |= (u_int64_t)1 << LED_PIN_BLUE;
    cfg.mode = GPIO_MODE_OUTPUT;
    cfg.pull_up_en = 0;
    cfg.pull_down_en = 0;
    gpio_config(&cfg);

    cfg.pin_bit_mask = (u_int64_t)1 << BUTTON_PIN_RED;
    cfg.pin_bit_mask |= (u_int64_t)1 << BUTTON_PIN_YELLOW;
    cfg.mode = GPIO_MODE_INPUT;
    cfg.pull_up_en = 1;
    cfg.pull_down_en = 0;
    gpio_config(&cfg);

    // you will need to use gpio_config()
}

/* switches LED A on if level!=0 or off if level==0*/
void setLEDA(uint8_t level)
{
    if (level)
    {
        // set the pin of LED A to ON
        gpio_set_level(LED_PIN_BLUE, 1); // you probably need to use gpio_set_level()
    }
    else
    {
        gpio_set_level(LED_PIN_BLUE, 0); // set the pin of LED A to OFF
    }
}

/* switches LED B on if level!=0 or off if level==0*/
void setLEDB(uint8_t level)
{
    if (level)
    {                                   // set the pin of LED A to ON
        gpio_set_level(LED_PIN_RED, 1); // you probably need to use gpio_set_level()
    }
    else
    {
        gpio_set_level(LED_PIN_RED, 0);
    }
}

/* tells if button A is currently being pressed */

uint8_t isButtonAPressed()
{
    return !gpio_get_level(BUTTON_PIN_RED);
}

/* tells if button A is currently being pressed */
uint8_t isButtonBPressed()
{
    return !gpio_get_level(BUTTON_PIN_YELLOW);
}