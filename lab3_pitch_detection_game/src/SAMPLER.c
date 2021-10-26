#include <esp_task_wdt.h>
#include <driver/adc.h>
#include <soc/adc_channel.h>
#include <esp32/rom/ets_sys.h>
#include <esp_task_wdt.h>
#include <driver/dac.h>
#include <soc/dac_channel.h>

esp_timer_handle_t timer;
int lastSample = 0;
int sampleCrossing = 0;
float frequency = 0;
uint64_t startTime;

void timerCallback()
{
    int sample = adc1_get_raw(ADC1_GPIO32_CHANNEL);

    if (lastSample < 1050 && sample > 1050)
    {
        sampleCrossing++;
    }
    else if (lastSample > 1050 && sample < 1050)
    {
        sampleCrossing++;
    }
    lastSample = sample;
    
    // 0 - 4096      0 - 3,3 V den max är 3,9 men kan bara ge 3,3
    // 4096/3,9 = 1050     (1050 = 1 V)
}

/*
 * Starts sampling on a given pin with a given fequency.
 * Parameter freq: the sampling frequency
*/
void startSampling(int freq)
{
    lastSample = 0;
    frequency = 0;
    sampleCrossing = 0;
    adc_power_acquire();
    adc_gpio_init(ADC_UNIT_1, ADC1_GPIO32_CHANNEL);
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_GPIO32_CHANNEL, ADC_ATTEN_DB_11);

    esp_timer_init();
    esp_timer_create_args_t config;
    config.name = "timer";
    config.arg = NULL;
    config.callback = &timerCallback;

    esp_err_t res = esp_timer_create(&config, &timer);
    ESP_ERROR_CHECK(res);
    int t = 1000000 / freq;
    res = esp_timer_start_periodic(timer, t);
    ESP_ERROR_CHECK(res);

    startTime = esp_timer_get_time();
}

/*
* Stops the sampling process.
*/
void stopSampling()
{
    esp_timer_stop(timer);
    int64_t endtime = esp_timer_get_time();
    int64_t periodicTime = (endtime - startTime);
    frequency = (1000000 * (float)sampleCrossing / (float) (2 * periodicTime));
    printf("Freq: %.2f\n", frequency);
}

/*
 * Get signalCrossing
 * 
*/
float getFrequency()
{
    return frequency;
}