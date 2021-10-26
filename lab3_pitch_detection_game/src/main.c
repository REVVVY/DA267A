#include "SOUNDGEN.h"
#include <stdio.h>
#include <esp32/rom/ets_sys.h>
#include <esp_task_wdt.h>
#include "SAMPLER.h"
#include"soundgen.h"
#include"sampler.h"
#include"pins.h"
#include "random.h"

float absval(float a)
{
    if (a < 0)
    {
        return -a;
    }
    else
    {
        return a;
    }
}
void app_main()
{

    initPin();

    while (1)
    {

        setLED(1);
        vTaskDelay(pdMS_TO_TICKS(250));
        setLED(0);
        vTaskDelay(pdMS_TO_TICKS(250));
        setLED(1);
        vTaskDelay(pdMS_TO_TICKS(250));
        setLED(0);

        int ranFreq = getRandommsecs(200, 1000);

        int actualFreq = startSound(ranFreq);

        vTaskDelay(pdMS_TO_TICKS(3000));

        stopSound();

        printf("---Start!\n");

        startSampling(4000);

        vTaskDelay(pdMS_TO_TICKS(2000));

        stopSampling();

        printf("---Stop!\n");

        float freq = getFrequency();

        printf("player: %.2f, actual: %d\n", freq, actualFreq);

        if (absval(freq - (float)actualFreq) < 50)
        {
            printf("Player wins!\n");
            // Light up for 5 seconds
            setLED(1);
            vTaskDelay(pdMS_TO_TICKS(5000));
            setLED(0);
        }

        else
        {
            printf("Player loses!\n");
            printf("Game over n00b, git gud\n");
        }
    }
}