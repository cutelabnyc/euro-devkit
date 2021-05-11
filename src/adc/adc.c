#include "adc.h"
#include "uexkull.h"

static void ADC_init_param(param_t *self, component_t component)
{
    self->component = component;
    self->val = 0;

    rampsmooth_init(&self->rampsmooth, ADC_BUFFER_LENGTH);

}

void ADC_init(adc_t *self, ADC_HandleTypeDef *adcx)
{
    // Initialize HAL ADC and ADC DMA buffer
    self->hadc = adcx;

    for (int i = 0; i < EURORACK_NUM_PARAMS; i++)
    {
        self->adcBuf[i] = 0;
    }

    // Initialize params one by one
    ADC_init_param(&self->fundamental, POTENTIOMETER_50K);
    ADC_init_param(&self->fineTune, POTENTIOMETER_50K);
    ADC_init_param(&self->diffractionConstant, POTENTIOMETER_50K);
    ADC_init_param(&self->numOsc, POTENTIOMETER_50K);

    self->counter = 0;
}

void ADC_processBlock(adc_t *self)
{
    self->diffractionConstant.val = ((float)self->adcBuf[2] / 256.0f) * 5;
    self->numOsc.val = ((float)self->adcBuf[3] / 256.0f) * NUM_OSC;

    // if (self->counter == ADC_BUFFER_LENGTH) {
    self->fundamental.val = (uint32_t)rampsmooth_process(&self->fundamental.rampsmooth);
    self->fineTune.val = (uint32_t)rampsmooth_process(&self->fineTune.rampsmooth);

    // self->counter = 0;
// }
// else {
    rampsmooth_appendSample(&self->fundamental.rampsmooth, self->adcBuf[0]);
    rampsmooth_appendSample(&self->fineTune.rampsmooth, self->adcBuf[1]);
    // self->counter++;
// }
}
