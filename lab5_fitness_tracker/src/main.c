#include "I2C.h"
#include <stdio.h>
#include <esp32/rom/ets_sys.h>
#include <esp_task_wdt.h>
#include <esp_pm.h>

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
#define MPU6050_Accel_Lsb 16384.f

void app_main()
{
    // initialise the I2C bus and the MPU6050
    initI2C(SDAPIN, SCLPIN);
    writeI2C(MPU6050_ADDR, MPU6050_PWR_MGMT_1, 0);
    writeI2C(MPU6050_ADDR, MPU6050_SMPLRT_DIV, 250);

    esp_pm_config_esp32_t config = {
        .light_sleep_enable = TRUE,
        .min_freq_mhz = 13, 
        .max_freq_mhz = 80,
    } 
    esp_pm_configure(&config);

    /*
    while (1)
    {
        uint8_t buffer;

        int16_t rawData = 0;
        readI2C(MPU6050_ADDR, MPU6050_TEMP_OUT_L, &buffer, 1);
        rawData = buffer;
        readI2C(MPU6050_ADDR, MPU6050_TEMP_OUT_H, &buffer, 1);
        rawData |= (uint16_t)buffer << 8;

        float temp = ((float)rawData) / 340 + 36.53;
        float xAccel, yAccel, zAccel;
        readI2C(MPU6050_ADDR, MPU6050_ACCEL_XOUT_L, &buffer, 1);
        rawData = buffer;
        readI2C(MPU6050_ADDR, MPU6050_ACCEL_XOUT_H, &buffer, 1);
        rawData |= (uint16_t)buffer << 8;
        xAccel = (float)rawData/MPU6050_Accel_Lsb;
        readI2C(MPU6050_ADDR, MPU6050_ACCEL_YOUT_L, &buffer, 1);
        rawData = buffer;
        readI2C(MPU6050_ADDR, MPU6050_ACCEL_YOUT_H, &buffer, 1);
        rawData |= (uint16_t)buffer << 8;
        yAccel = (float)rawData/MPU6050_Accel_Lsb;
        readI2C(MPU6050_ADDR, MPU6050_ACCEL_ZOUT_L, &buffer, 1);
        rawData = buffer;
        readI2C(MPU6050_ADDR, MPU6050_ACCEL_ZOUT_H, &buffer, 1);
        rawData |= (uint16_t)buffer << 8;
        zAccel = (float)rawData/MPU6050_Accel_Lsb;
        printf("temperature is: %.2f C, x: %.2f, y: %.2f, z: %.2f,  \n", temp, xAccel, yAccel, zAccel);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
    */
}
