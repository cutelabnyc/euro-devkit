#ifndef __ADC_H
#define __ADC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f7xx_hal.h"

    typedef enum component {
        POTENTIOMETER,
        SWITCH,
        LED,
        INLET
    } component_t;

    typedef struct param {
        component_t component;
        GPIO_InitTypeDef GPIO_InitStruct;
        uint16_t val; // TODO: Like in the rampsmooth class, how on earth do I make this mutable
    } param_t;

    typedef struct adc {
        ADC_HandleTypeDef *hadc;

        param_t freq;
        param_t numOsc;

    } adc_t;

    void ADC_init(adc_t *self, ADC_HandleTypeDef *adcx);
    void ADC_init_param(param_t *self, component_t component);
    void ADC_processBlock(adc_t *self);

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H */
