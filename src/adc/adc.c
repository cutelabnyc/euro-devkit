#include "adc.h"

static void ADC_init_param(param_t *self, component_t component)
{
    self->component = component;

    self->val = 0;
}

void ADC_init(adc_t *self, ADC_HandleTypeDef *adcx)
{
    // Initialize HAL ADC and ADC DMA buffer
    self->hadc = adcx;

    for (int i = 0; i < NUM_PARAMS; i++)
    {
        self->adcBuf[i] = 0;
    }

    // Initialize params one by one
    ADC_init_param(&self->fundamental, POTENTIOMETER_256K);
    ADC_init_param(&self->fineTune, POTENTIOMETER_256K);

    // Initialize other signal processing components
    // rampsmooth_init(&self->rampsmooth, 4);
}

void ADC_processBlock(adc_t *self)
{
    self->fundamental.val = self->adcBuf[0];
    self->fineTune.val = self->adcBuf[1];
}
