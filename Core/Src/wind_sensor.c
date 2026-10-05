#include "wind_sensor.h"

float Read_Wind_Speed(ADC_HandleTypeDef *hadc) {
    uint32_t adc_sum = 0;

    for (uint8_t i = 0; i < WIND_ADC_OVERSAMPLE_COUNT; i++) {
        HAL_ADC_Start(hadc);
        if (HAL_ADC_PollForConversion(hadc, 10) == HAL_OK) {
            adc_sum += HAL_ADC_GetValue(hadc);
        }
        HAL_ADC_Stop(hadc);
    }
    uint32_t adc_val = adc_sum / WIND_ADC_OVERSAMPLE_COUNT;

    float pin_voltage = ((float)adc_val / ADC_MAX) * VREF;
    float sensor_voltage = pin_voltage * VOLTAGE_DIVIDER_RATIO;
    float wind_speed = (sensor_voltage / 5.0f) * MAX_WIND_SPEED;

    if (wind_speed < 0.0f) wind_speed = 0.0f;
    return wind_speed;
}
