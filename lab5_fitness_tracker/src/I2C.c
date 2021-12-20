#include "I2C.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdint.h>
#include "esp_system.h"

// I2C driver
#include "driver/i2c.h"

// Error library
#include "esp_err.h"

void initI2C(int sdapin, int sclpin)
{
    i2c_config_t config = {
        .mode = I2C_MODE_MASTER, // ESP32 acts as master
        .sda_io_num = sdapin, // pin used for SDA
        .scl_io_num = sclpin, // SCL pin number
        .sda_pullup_en = GPIO_PULLUP_ENABLE, // The SDA and SCL lines are active low, so they should be pulled up with resistors
        .scl_pullup_en = GPIO_PULLUP_ENABLE, // enables pullup on SDA
        .master.clk_speed = 100000 // Standard mode (100 Kbit/s)
    };
    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &config));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, config.mode, 0, 0, 0));
}

/**
* Writes one byte into a register
*/
void writeI2C(uint8_t address, uint8_t reg, uint8_t data)
{
    // create command
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    // start command
    esp_err_t res = i2c_master_start(cmd);
    ESP_ERROR_CHECK(res);
    // set address + write and check for ack
    res = i2c_master_write_byte(cmd, address << 1 | I2C_MASTER_WRITE, 1);
    ESP_ERROR_CHECK(res);
    // write the register address and check for ack
    res = i2c_master_write_byte(cmd, reg, 1);
    ESP_ERROR_CHECK(res);
    res = i2c_master_write_byte(cmd, data, 1);
    ESP_ERROR_CHECK(res);
    // end of command
    res = i2c_master_stop(cmd);
    ESP_ERROR_CHECK(res);
    // send the command, 1 second timeout
    res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    ESP_ERROR_CHECK(res);
    // delete command now that it's not needed
    i2c_cmd_link_delete(cmd);
}
/**
* Reads len bytes and places them into a buffer, buffer must be pre-allocated
*/
void readI2C(uint8_t address, uint8_t reg, uint8_t *buffer, int len)
{
    // read low register
    // send just the register number with no other data
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    cmd = i2c_cmd_link_create();
    esp_err_t res = i2c_master_start(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_write_byte(cmd, address << 1 | I2C_MASTER_WRITE, 1); // WRITE bit set!
    ESP_ERROR_CHECK(res);
    res = i2c_master_write_byte(cmd, reg, 1); // read low first
    ESP_ERROR_CHECK(res);
    res = i2c_master_stop(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    ESP_ERROR_CHECK(res);
    i2c_cmd_link_delete(cmd);

    // read high register
    cmd = i2c_cmd_link_create();
    res = i2c_master_start(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_write_byte(cmd, address << 1 | I2C_MASTER_READ, 1); // WRITE bit set!
    ESP_ERROR_CHECK(res);
    res = i2c_master_read(cmd, buffer, len, I2C_MASTER_NACK); // read high
    ESP_ERROR_CHECK(res);
    res = i2c_master_stop(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    ESP_ERROR_CHECK(res);
    i2c_cmd_link_delete(cmd);
}