#ifndef __ADC_H
#define __ADC_H

#ifdef __cplusplus
extern "C"
{
#endif

#define EURORACK_NUM_PARAMS 4
#define ADC_BUFFER_LENGTH 2
#define ADC_BIT_DEPTH 4096.0f

#include "stm32f7xx_hal.h"
#include <cuteop.h>

    // TODO: This could be for a future API in which
    // the type of val in param_t could be solely based
    // on which component is used in any given module's
    // hardware, instead of everything being uint16_t
    typedef enum component {
        POTENTIOMETER_10K,
        POTENTIOMETER_50K,
        SWITCH,
        LED,
        INLET
    } component_t;

    // TODO: GPIO values are currently initialized in
    // stm32f7xx_hal_msp.c, but it would be more efficient
    // to centralize GPIO initialization to this file
    typedef struct param {
        component_t component;
        t_fbsmooth fbsmooth;
        // GPIO_InitTypeDef GPIO_InitStruct;
        uint32_t val;
    } param_t;

    typedef struct adc {
        ADC_HandleTypeDef *hadc;

        // NOTE: ADC elements are initialized in the order 
        // in which they are initialized in MX_ADC_Init()
        uint32_t adcBuf[EURORACK_NUM_PARAMS];
        uint8_t muxSelect;
        param_t fundamental;
        param_t fineTune;
        param_t diffractionConstant;
        param_t numOsc;
        // param_t logLin;
        // param_t waveform;
        // param_t lfoFreq;
        // param_t lfoPhaseOffset
        // param_t lfoFreqOffset

    } adc_t;

    void ADC_init(adc_t *self, ADC_HandleTypeDef *adcx);
    void ADC_processBlock(adc_t *self);

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H */
