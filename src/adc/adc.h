#ifndef __ADC_H
#define __ADC_H

#ifdef __cplusplus
extern "C"
{
#endif

#define FILTER_BUFFER_LENGTH 2
#define ADC_BIT_DEPTH 4096.0f

#include "stm32f7xx_hal.h"
#include "globals.h"
#include <cuteop.h>

    uint32_t dma_buf[NUM_UX_ADC_PARAMS];

    typedef struct adc_param {
        uint8_t id;
        t_fbsmooth fbsmooth;
        uint32_t val;
    } adc_param_t;

    typedef struct gpio_param {
        uint8_t id;
        bool val;
    } gpio_param_t;

    /* TODO: Implement LED Params struct */

    typedef struct mux_param {
        uint8_t sel;
        adc_param_t params[NUM_UX_MUX_PARAMS];
    } mux_param_t;

    typedef struct adc {
        ADC_HandleTypeDef *hadc;
        uint8_t interSelector;
        adc_param_t adc_params[NUM_UX_ADC_PARAMS];
        gpio_param_t gpio_params[NUM_UX_GPIO_PARAMS];
        // led_param_t led_params[NUM_UX_LED_PARAMS];
        mux_param_t mux[NUM_UX_MUXS];

    } adc_t;


    void ADC_init(adc_t *self, ADC_HandleTypeDef *adcx);
    void ADC_processBlock(adc_t *self);

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H */
