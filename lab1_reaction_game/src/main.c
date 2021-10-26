#include <esp_task_wdt.h>
#include "pins.h"
#include "random.h"

void waitMs(unsigned int millis)
{
    TickType_t delay = millis / portTICK_PERIOD_MS;
    vTaskDelay(delay);
}

/*Waits for "millis" milliseconds without upsetting the watchdog timer*/

void app_main()
{
    initPins();
    
    while(1){

        setLEDA(1);
        setLEDB(1);
        waitMs(250);
        setLEDA(0);
        setLEDB(0);
        waitMs(250);
        setLEDA(1);
        setLEDB(1);
        waitMs(250);
        setLEDA(0);
        setLEDB(0);
        waitMs(getRandommsecs(3000, 5000));
        setLEDA(1);
        setLEDB(1);

        // you can flash LEDs in a certain way for it
        // switch both LEDs off
        // get a random duration between 3 and 5 seconds
        // wait that random duration
        // switch both LEDs ON

        uint8_t winner = 0;
        while(!winner){

            if(isButtonAPressed()){
                winner = 1;
            } else if(isButtonBPressed()){
                winner = 2;
            }
            // check if either button A or B are pressed

            // if any is pressed, set winner to 1 for A or 2 for B
        }
        if (winner == 1)
        {
            setLEDA(1);
            setLEDB(0);
            waitMs(5000);
        } else if (winner == 2){
            setLEDB(1);
            setLEDA(0);
            waitMs(5000);
        }
        setLEDA(0);
        setLEDB(0);
        waitMs(5000);
        app_main();
        // if A wins, flash LED A and switch off B

        // if B wins, flash LED B and switch off A

        // switch off both A and B and wait for some time to restart the game
    }
}