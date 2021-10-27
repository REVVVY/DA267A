#include "I2C.h"
#include "circular_buffer.h"
#include <stdio.h>
#include <esp32/rom/ets_sys.h>
#include <esp_task_wdt.h>
#include <esp_pm.h>
#include <stdint.h>
#include "math.h"

#define SDAPIN 33
#define SCLPIN 32
#define MPU6050_ADDR 0x68         // address
#define MPU6050_PWR_MGMT_1 0x6B   // register
#define MPU6050_SMPLRT_DIV 0x19   // register
#define MPU6050_WHO_AM_I 0x75     // register
#define MPU6050_TEMP_OUT_H 0x41   // temperatur-register
#define MPU6050_TEMP_OUT_L 0x42   // temperatur-register
#define MPU6050_ACCEL_XOUT_H 0x3B //accelerator x-axel-register
#define MPU6050_ACCEL_XOUT_L 0x3C //accelerator x-axel-register
#define MPU6050_ACCEL_YOUT_H 0x3D //accelerator y-axel-register
#define MPU6050_ACCEL_YOUT_L 0x3E //accelerator y-axel-register
#define MPU6050_ACCEL_ZOUT_H 0x3F //accelerator z-axel-register
#define MPU6050_ACCEL_ZOUT_L 0x40 //accelerator z-axel-register
#define BUFFER_SIZE 10
#define SAMPLING_PERIOD 150
#define STEP_TASK 3000

float k = 0.6;

int realSteps = 0;

uint32_t acc;
struct circularBuffer circularBuffer;
uint32_t *buffer_data;

void accelerationTask(void *arg)
{
    TickType_t xLastWake = xTaskGetTickCount();
    while (1)
    {
        uint8_t buffer;
        int16_t rawData = 0;
        float xAccel, yAccel, zAccel;
        readI2C(MPU6050_ADDR, MPU6050_ACCEL_XOUT_L, &buffer, 1);
        rawData = buffer;
        readI2C(MPU6050_ADDR, MPU6050_ACCEL_XOUT_H, &buffer, 1);
        rawData |= (uint16_t)buffer << 8;
        xAccel = (float)rawData;
        readI2C(MPU6050_ADDR, MPU6050_ACCEL_YOUT_L, &buffer, 1);
        rawData = buffer;
        readI2C(MPU6050_ADDR, MPU6050_ACCEL_YOUT_H, &buffer, 1);
        rawData |= (uint16_t)buffer << 8;
        yAccel = (float)rawData;
        readI2C(MPU6050_ADDR, MPU6050_ACCEL_ZOUT_L, &buffer, 1);
        rawData = buffer;
        readI2C(MPU6050_ADDR, MPU6050_ACCEL_ZOUT_H, &buffer, 1);
        rawData |= (uint16_t)buffer << 8;
        zAccel = (float)rawData;

        acc = (uint32_t)sqrtf(xAccel * xAccel + yAccel * yAccel + zAccel * zAccel);

        addElement(&circularBuffer, acc);
        vTaskDelayUntil(&xLastWake, pdMS_TO_TICKS(SAMPLING_PERIOD));
    }
}

void stepTask(void *arg)
{
    TickType_t xLastWake = xTaskGetTickCount();
    while (1)
    {
        if (!isEmpty(&circularBuffer))
        {
            int size = 0;
            float meanValue = 0;
            float val = 0;
            float sum = 0;
            float sd = 0;
            for (size_t i = 0; i < circularBuffer.maxLength; i++)
            {
                if (circularBuffer.data[i] != UINT32_MAX)
                {
                    val = val + circularBuffer.data[i];
                    size++;
                }
            }
            meanValue = (val) / (size);
            //printBuffer(&circularBuffer);
            for (size_t i = 0; i < circularBuffer.maxLength; i++)
            {
                if (circularBuffer.data[i] != UINT32_MAX)
                {
                    sum = sum + powf((circularBuffer.data[i] - meanValue), 2);
                }
            }
            sum = sum / size;
            sd = sqrtf(sum);

            if (sd > 800)
            {
                for (size_t i = 0; i < circularBuffer.maxLength; i++)
            {
                if (circularBuffer.data[i] != UINT32_MAX)
                {
                    if ((float)removeHead(&circularBuffer) > meanValue + (k * sd))
                    {
                        realSteps++;
                    }
                }
            }
            }
            printf("meanValue: %d\n", (int)meanValue);
            printf("SD: %d\n", (int)sd);
            printf("Steps: %d\n", realSteps);
        }
        vTaskDelayUntil(&xLastWake, pdMS_TO_TICKS(STEP_TASK));
    }
}

void app_main()
{
    esp_pm_config_esp32_t config = {
        .light_sleep_enable = true,
        .min_freq_mhz = 14,
        .max_freq_mhz = 80};

    esp_pm_configure(&config);

    // initialise the I2C bus and the MPU6050
    initI2C(SDAPIN, SCLPIN);
    writeI2C(MPU6050_ADDR, MPU6050_PWR_MGMT_1, 0);
    writeI2C(MPU6050_ADDR, MPU6050_SMPLRT_DIV, 250);

    buffer_data = (uint32_t *)malloc(BUFFER_SIZE * sizeof(uint32_t));
    initCircularBuffer(&circularBuffer, buffer_data, BUFFER_SIZE);

    xTaskCreate(accelerationTask, "AcceleratorTask", 2048, NULL, 1, NULL);
    xTaskCreate(stepTask, "stepTask", 2048, NULL, 2, NULL);
}
