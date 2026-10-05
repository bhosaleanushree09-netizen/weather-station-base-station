#include "bme680_sensor.h"

struct bme68x_dev bme_dev;
struct bme68x_conf bme_conf;
struct bme68x_heatr_conf bme_heatr_conf;
struct bme68x_data bme_data;
uint8_t g_bme_sensor_ready = 0;

static I2C_HandleTypeDef *bme_hi2c_ptr;

static BME68X_INTF_RET_TYPE bme68x_i2c_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr) {
    uint8_t dev_addr = *(uint8_t *)intf_ptr;
    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(bme_hi2c_ptr, (dev_addr << 1), reg_addr,
                                                I2C_MEMADD_SIZE_8BIT, reg_data, (uint16_t)len, 100);
    return (status == HAL_OK) ? BME68X_OK : BME68X_E_COM_FAIL;
}

static BME68X_INTF_RET_TYPE bme68x_i2c_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr) {
    uint8_t dev_addr = *(uint8_t *)intf_ptr;
    HAL_StatusTypeDef status = HAL_I2C_Mem_Write(bme_hi2c_ptr, (dev_addr << 1), reg_addr,
                                                 I2C_MEMADD_SIZE_8BIT, (uint8_t *)reg_data, (uint16_t)len, 100);
    return (status == HAL_OK) ? BME68X_OK : BME68X_E_COM_FAIL;
}

static void bme68x_delay_us(uint32_t period, void *intf_ptr) {
    uint32_t ms = period / 1000;
    if (ms == 0) ms = 1;
    HAL_Delay(ms);
}

void Init_BME680(I2C_HandleTypeDef *hi2c) {
    bme_hi2c_ptr = hi2c;
    int8_t rslt;
    static uint8_t i2c_addrs[] = {BME68X_I2C_ADDR_LOW, BME68X_I2C_ADDR_HIGH};

    HAL_Delay(100);

    for (uint8_t i = 0; i < 2; i++) {
        bme_dev.intf = BME68X_I2C_INTF;
        bme_dev.read = bme68x_i2c_read;
        bme_dev.write = bme68x_i2c_write;
        bme_dev.delay_us = bme68x_delay_us;
        bme_dev.intf_ptr = &i2c_addrs[i];
        bme_dev.amb_temp = 25;

        rslt = bme68x_init(&bme_dev);
        if (rslt == BME68X_OK) {
            g_bme_sensor_ready = 1;
            break;
        }
    }

    if (!g_bme_sensor_ready) return;

    bme_conf.os_hum = BME68X_OS_2X;
    bme_conf.os_pres = BME68X_OS_4X;
    bme_conf.os_temp = BME68X_OS_8X;
    bme_conf.filter = BME68X_FILTER_SIZE_3;
    bme_conf.odr = BME68X_ODR_NONE;
    bme68x_set_conf(&bme_conf, &bme_dev);

    bme_heatr_conf.enable = BME68X_ENABLE;
    bme_heatr_conf.heatr_temp = 300;
    bme_heatr_conf.heatr_dur = 150;
    bme68x_set_heatr_conf(BME68X_FORCED_MODE, &bme_heatr_conf, &bme_dev);
}
