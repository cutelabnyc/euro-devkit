#ifndef __ADC_H
#define __ADC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f7xx_hal.h"

    // typedef struct gpio {
    //     float val;
    //     // GPIO_pin...
    // } gpio_t;

    typedef struct adc {
        ADC_HandleTypeDef *hadc;
        float adcValue;
    } adc_t;

    void ADC_init(adc_t *self, ADC_HandleTypeDef *adcx);
    void ADC_processBlock(adc_t *self);

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H */
