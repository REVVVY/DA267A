#include <stdio.h>
#include <stdlib.h>
#include <esp32/rom/ets_sys.h>
#include <esp_task_wdt.h>
#include "driver/gpio.h"

#include "double_linked_list.h"

#define LED_PIN_LEVEL_UP 12
#define LED_PIN_LEVEL_MIDDLE 14
#define LED_PIN_LEVEL_DOWN 27
#define BUTTON_PIN 26

#define PUSH_TIME_US 250000 // 250 ms

// Used to represent a travel need of a passenger.
struct travel_need
{
    int origin;      //Där dem väntar
    int destination; //Dit dem ska
};

//LinkedList for travelers
struct doubleLinkedList doubleList;

// Integers to keep the elevator busy
int origin = INT_MAX;      //Current travelers origin
int destination = INT_MAX; //Current travelers destination
int level = INT_MIN;       //Level the elevator should go to
int readyForNext = 0;      //Check if next traveler can be sent

// Used to not allow button pushes that are too close to each other in time
static volatile uint64_t lastPush = -PUSH_TIME_US; // Kontrollera

//Just a counter keeping track of which travel need is next to process.
static volatile int travel_need_counter = 0; //Orginal 0

// This data structure holds information about
static volatile struct travel_need travel_needs[50];

// This function is called when button is pushed
static void handle_push(void *arg)
{

    // Disable interrupts
    gpio_intr_disable(BUTTON_PIN);

    // Get the current time
    uint64_t now = esp_timer_get_time();

    // If enough time passed, we should consider this event as a genuine push
    if ((now - lastPush) > PUSH_TIME_US)
    {
        lastPush = now;
        //Get next travel need from list and do something with it

        addElementDoubleLinkedList(&doubleList, travel_need_counter);
        //Now we are just blinking the LEDs. You need to update this code.
        //uint32_t level = travel_needs[travel_need_counter].origin % 4; //travel_need_counter
        travel_need_counter++;
    }
    // Re-enable interrupts
    gpio_intr_enable(BUTTON_PIN);
}

void app_main()
{
    //Initialize double linkedlist
    initDoubleLinkedList(&doubleList);

    //Initialize travel needs (50 randomly generated travel needs)
    travel_needs[0].origin = 2;
    travel_needs[0].destination = 1;
    travel_needs[1].origin = 1;
    travel_needs[1].destination = 2;
    travel_needs[2].origin = 1;
    travel_needs[2].destination = 2;
    travel_needs[3].origin = 0;
    travel_needs[3].destination = 2;
    travel_needs[4].origin = 2;
    travel_needs[4].destination = 1;
    travel_needs[5].origin = 0;
    travel_needs[5].destination = 2;
    travel_needs[6].origin = 1;
    travel_needs[6].destination = 2;
    travel_needs[7].origin = 1;
    travel_needs[7].destination = 0;
    travel_needs[8].origin = 0;
    travel_needs[8].destination = 1;
    travel_needs[9].origin = 1;
    travel_needs[9].destination = 0;
    travel_needs[10].origin = 1;
    travel_needs[10].destination = 2;
    travel_needs[11].origin = 0;
    travel_needs[11].destination = 1;
    travel_needs[12].origin = 0;
    travel_needs[12].destination = 2;
    travel_needs[13].origin = 0;
    travel_needs[13].destination = 1;
    travel_needs[14].origin = 0;
    travel_needs[14].destination = 2;
    travel_needs[15].origin = 0;
    travel_needs[15].destination = 1;
    travel_needs[16].origin = 2;
    travel_needs[16].destination = 1;
    travel_needs[17].origin = 2;
    travel_needs[17].destination = 1;
    travel_needs[18].origin = 1;
    travel_needs[18].destination = 0;
    travel_needs[19].origin = 2;
    travel_needs[19].destination = 1;
    travel_needs[20].origin = 1;
    travel_needs[20].destination = 0;
    travel_needs[21].origin = 0;
    travel_needs[21].destination = 1;
    travel_needs[22].origin = 1;
    travel_needs[22].destination = 2;
    travel_needs[23].origin = 0;
    travel_needs[23].destination = 2;
    travel_needs[24].origin = 2;
    travel_needs[24].destination = 1;
    travel_needs[25].origin = 1;
    travel_needs[25].destination = 0;
    travel_needs[26].origin = 1;
    travel_needs[26].destination = 2;
    travel_needs[27].origin = 0;
    travel_needs[27].destination = 2;
    travel_needs[28].origin = 1;
    travel_needs[28].destination = 0;
    travel_needs[29].origin = 1;
    travel_needs[29].destination = 2;
    travel_needs[30].origin = 0;
    travel_needs[30].destination = 1;
    travel_needs[31].origin = 1;
    travel_needs[31].destination = 2;
    travel_needs[32].origin = 0;
    travel_needs[32].destination = 2;
    travel_needs[33].origin = 0;
    travel_needs[33].destination = 2;
    travel_needs[34].origin = 1;
    travel_needs[34].destination = 2;
    travel_needs[35].origin = 2;
    travel_needs[35].destination = 1;
    travel_needs[36].origin = 0;
    travel_needs[36].destination = 2;
    travel_needs[37].origin = 1;
    travel_needs[37].destination = 0;
    travel_needs[38].origin = 0;
    travel_needs[38].destination = 2;
    travel_needs[39].origin = 2;
    travel_needs[39].destination = 1;
    travel_needs[40].origin = 0;
    travel_needs[40].destination = 1;
    travel_needs[41].origin = 0;
    travel_needs[41].destination = 1;
    travel_needs[42].origin = 0;
    travel_needs[42].destination = 1;
    travel_needs[43].origin = 1;
    travel_needs[43].destination = 0;
    travel_needs[44].origin = 0;
    travel_needs[44].destination = 2;
    travel_needs[45].origin = 2;
    travel_needs[45].destination = 1;
    travel_needs[46].origin = 2;
    travel_needs[46].destination = 1;
    travel_needs[47].origin = 2;
    travel_needs[47].destination = 1;
    travel_needs[48].origin = 0;
    travel_needs[48].destination = 2;
    travel_needs[49].origin = 1;
    travel_needs[49].destination = 0;

    gpio_config_t config;

    // Configure pin LED_PIN_LEVEL_UP as output
    config.pin_bit_mask = (u_int64_t)1 << LED_PIN_LEVEL_UP;
    config.mode = GPIO_MODE_OUTPUT;
    config.pull_up_en = 0;
    config.pull_down_en = 0;
    gpio_config(&config);

    // Configure pin LED_PIN_LEVEL_MIDDLE as output
    config.pin_bit_mask = (u_int64_t)1 << LED_PIN_LEVEL_MIDDLE;
    config.mode = GPIO_MODE_OUTPUT;
    config.pull_up_en = 0;
    config.pull_down_en = 0;
    gpio_config(&config);

    // Configure pin LED_PIN_LEVEL_DOWN as output
    config.pin_bit_mask = (u_int64_t)1 << LED_PIN_LEVEL_DOWN;
    config.mode = GPIO_MODE_OUTPUT;
    config.pull_up_en = 0;
    config.pull_down_en = 0;
    gpio_config(&config);

    // Configure pin BUTTON_PIN as input, pull up and with interrupts on the negative edge
    config.pin_bit_mask = (u_int64_t)1 << BUTTON_PIN;
    config.mode = GPIO_MODE_INPUT;
    config.pull_up_en = 1;
    config.pull_down_en = 0;
    config.intr_type = GPIO_INTR_NEGEDGE;
    gpio_config(&config);

    // Activate the interrupts for the GPIOs
    esp_err_t res = gpio_install_isr_service(0);
    ESP_ERROR_CHECK(res);

    // Add a handler to the ISR for pin BUTTON_PIN
    res = gpio_isr_handler_add(BUTTON_PIN, handle_push, NULL);
    ESP_ERROR_CHECK(res);

    //gpio_set_level(LED_PIN_LEVEL_DOWN, 1); //Elevator stats from the bottom

    // This is where you most likely put your main elevator code.
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(150));

        if (readyForNext == 0 && doubleList.first != NULL)
        {
            readyForNext = 1;
        }

        if (readyForNext == 1)
        {
            //If origin and dest is empty
            if (origin != INT_MAX || destination != INT_MAX)
            {
                if (origin != INT_MAX)
                {
                    level = origin;
                    printf("Origin: %d\n", origin);
                    origin = INT_MAX;
                }
                else if (destination != INT_MAX)
                {
                    level = destination;
                    printf("Destination: %d\n", destination);
                    destination = INT_MAX;
                    removeFirstElementDoubleLinkedList(&doubleList);
                    readyForNext = 0;
                }
            }
            else if (origin == INT_MAX && destination == INT_MAX)
            {
                struct travel_need current_travel_need = travel_needs[doubleList.first->data];
                origin = current_travel_need.origin;
                destination = current_travel_need.destination;
                printf("Origin: %d, Destination: %d\n", origin, destination);
            }
        }
        printf("Level: %d\n", level);
        if (level == 0) //IFALL DEN ÄR 0 UP
        {
            gpio_set_level(LED_PIN_LEVEL_UP, 1);
            gpio_set_level(LED_PIN_LEVEL_MIDDLE, 0);
            gpio_set_level(LED_PIN_LEVEL_DOWN, 0);
            vTaskDelay(pdMS_TO_TICKS(5000));
            level = INT_MIN;
        }

        else if (level == 1) //IFALL DEN ÄR 1 MIDDLE
        {
            gpio_set_level(LED_PIN_LEVEL_UP, 0);
            gpio_set_level(LED_PIN_LEVEL_MIDDLE, 1);
            gpio_set_level(LED_PIN_LEVEL_DOWN, 0);
            vTaskDelay(pdMS_TO_TICKS(5000));
            level = INT_MIN;
        }

        else if (level == 2)
        {
            gpio_set_level(LED_PIN_LEVEL_UP, 0);
            gpio_set_level(LED_PIN_LEVEL_MIDDLE, 0);
            gpio_set_level(LED_PIN_LEVEL_DOWN, 1);
            vTaskDelay(pdMS_TO_TICKS(5000));
            level = INT_MIN;
        }
        else
        {
            printf("Do nothing\n");
        }
    }
    //ISR = AVBROTT
    //Den ska enbart hämta strukten för just den personen man är på och spara den i kön
    //While ska kolla ifall kön har personer i sig isf utföra if satsen annars bara fortsätta rulla utan avbrott.
}