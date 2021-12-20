#include "I2C.h"
#include "circular_buffer.h"
#include <stdio.h>
#include <esp32/rom/ets_sys.h>
#include <esp_task_wdt.h>
#include <esp_pm.h>
#include <stdint.h>
#include "math.h"

#define SDAPIN 33                 //PIN
#define SCLPIN 32                 //PIN
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
#define STEP_TASK 2000

float k = 1.01;

int realSteps = 0;

uint32_t acc;
struct circularBuffer circularBuffer;
uint32_t *buffer_data;

void accelerationTask(void *arg)
{
    TickType_t xLastWake = xTaskGetTickCount();
    while (1)
    {
        uint8_t buffer; //Buffer to add in the raw data 
        int16_t rawData = 0; //raw data
        float xAccel, yAccel, zAccel; //accel variables for all the axels x,y,z
        readI2C(MPU6050_ADDR, MPU6050_ACCEL_XOUT_L, &buffer, 1);
        rawData = buffer; //example 0000 0000 1111 1111 adds into first 8 bits (low bits)
        readI2C(MPU6050_ADDR, MPU6050_ACCEL_XOUT_H, &buffer, 1);
        rawData |= (uint16_t)buffer << 8; //example 1111 1111 xxxx xxxx shifted 8 steps so they get added into (high bits)
        xAccel = (float)rawData; //rawData is then added into xAccel until
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

        acc = (uint32_t)sqrtf(xAccel * xAccel + yAccel * yAccel + zAccel * zAccel); //Adds all of them together into acc to add in buffer

        addElement(&circularBuffer, acc); //Adds acc in buffer for steptask
        vTaskDelayUntil(&xLastWake, pdMS_TO_TICKS(SAMPLING_PERIOD)); 
    }
}

void stepTask(void *arg)
{
    TickType_t xLastWake = xTaskGetTickCount();
    while (1)
    {
        if (!isEmpty(&circularBuffer)) //Checks if buffer contains values from accelerationtask
        {
            //Variable used to keep values temporarily
            int size = 0;
            float meanValue = 0;
            float val = 0;
            float sum = 0;
            float sd = 0;
            for (size_t i = 0; i < circularBuffer.maxLength; i++) //Looping the buffer 
            {
                if (circularBuffer.data[i] != UINT32_MAX) //Check if data is not empty (data == UINT32_MAX if empty)
                {
                    val = val + circularBuffer.data[i]; //add data to val 
                    size++;
                }
            }
            meanValue = (val) / (size); //meanvalue of the data in buffer
            for (size_t i = 0; i < circularBuffer.maxLength; i++) //looping the buffer again 
            {
                if (circularBuffer.data[i] != UINT32_MAX) //Check if data is not empty (data == UINT32_MAX if empty)
                {
                    sum = sum + powf((circularBuffer.data[i] - meanValue), 2); 
                }
            }
            sum = sum / size;
            sd = sqrtf(sum);  
            // sd = sqrt((acc-meanValue)^2/size)

            if (sd > 400) //sd = 300 very little movement when walking sd goes to 3000 ish
            {
                for (size_t i = 0; i < circularBuffer.maxLength; i++) // looping the buffer to find the actual real steps
                {
                    if (circularBuffer.data[i] != UINT32_MAX)  //ignores if empty
                    {
                        if ((float)removeHead(&circularBuffer) > meanValue + (k * sd)) //if data in buffer > (k*sd) its a real step
                        {
                            realSteps++;
                        }
                    }
                }
            }
            //Printing everytime even if there is no new steps
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
        .min_freq_mhz = 14, //instructions said 13 but didn't work
        .max_freq_mhz = 80}; //instructions said 80

    esp_pm_configure(&config);

    // initialise the I2C bus and the MPU6050
    initI2C(SDAPIN, SCLPIN); //Init I2C
    writeI2C(MPU6050_ADDR, MPU6050_PWR_MGMT_1, 0); //setting all bits in reg PWR_MGMT to 0
    writeI2C(MPU6050_ADDR, MPU6050_SMPLRT_DIV, 250); //In MPU6050 Samling register set divider 250 which leads to a sampling freq of 32 Hz  

    buffer_data = (uint32_t *)malloc(BUFFER_SIZE * sizeof(uint32_t)); //memory for buffer
    initCircularBuffer(&circularBuffer, buffer_data, BUFFER_SIZE); //Buffer init

    xTaskCreate(accelerationTask, "AcceleratorTask", 2048, NULL, 1, NULL);  //accelerationTask priority 1
    xTaskCreate(stepTask, "stepTask", 2048, NULL, 2, NULL); //Step task priority 2
}
