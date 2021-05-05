#ifndef __ADC_H
#define __ADC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f7xx_hal.h"
#include <cuteop.h>

    typedef enum component {
        POTENTIOMETER,
        SWITCH,
        LED,
        INLET
    } component_t;

    // typedef enum valuetype {
    //     UINT16_T,
    //     UINT8_T
    // } valuetype_t;

    typedef struct param {
        component_t component;
        GPIO_InitTypeDef GPIO_InitStruct;
        // valuetype_t type;
        // union val {
        //     uint8_t val_uint8;
        //     uint16_t val_uint16;
        // };
        // uint16_t val; 
        // TODO: Like in the rampsmooth class, how on earth do I make this mutable
    } param_t;

    typedef struct adc {
        ADC_HandleTypeDef *hadc;
        t_rampsmooth rampsmooth;

        param_t freq;
        param_t numOsc;
        uint32_t adcValArray[2];
    } adc_t;

    void ADC_init(adc_t *self, ADC_HandleTypeDef *adcx);
    // static void ADC_init_param(param_t *self, component_t component);
    void ADC_processBlock(adc_t *self);

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H */
