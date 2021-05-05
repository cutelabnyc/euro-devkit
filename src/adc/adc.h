#ifndef __ADC_H
#define __ADC_H

#ifdef __cplusplus
extern "C"
{
#endif

#define NUM_PARAMS 2

#include "stm32f7xx_hal.h"
#include <cuteop.h>

    // TODO: This could be for a future API in which
    // the type of val in param_t could be solely based
    // on which component is used in any given module's
    // hardware, instead of everything being uint16_t
    typedef enum component {
        POTENTIOMETER_128K,
        POTENTIOMETER_256K,
        SWITCH,
        LED,
        INLET
    } component_t;

    // TODO: GPIO values are currently initialized in
    // stm32f7xx_hal_msp.c, but it would be more efficient
    // to centralize GPIO initialization to this file
    typedef struct param {
        component_t component;
        // GPIO_InitTypeDef GPIO_InitStruct;
        uint16_t val;
    } param_t;

    typedef struct adc {
        ADC_HandleTypeDef *hadc;
        uint32_t adcBuf[NUM_PARAMS];
        // t_rampsmooth rampsmooth;

        param_t fundamental;
        param_t fineTune;
    } adc_t;

    void ADC_init(adc_t *self, ADC_HandleTypeDef *adcx);
    void ADC_processBlock(adc_t *self);

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H */
