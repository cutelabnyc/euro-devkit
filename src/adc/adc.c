#include "adc.h"
#include "uexkull.h"

static void ADC_init_param(param_t *self, component_t component)
{
    self->component = component;
    self->val = 0;
    fbsmooth_init(&self->fbsmooth);

}

void ADC_init(adc_t *self, ADC_HandleTypeDef *adcx)
{
    // Initialize HAL ADC and ADC DMA buffer
    self->hadc = adcx;

    for (int i = 0; i < EURORACK_NUM_PARAMS; i++)
    {
        self->adcBuf[i] = 0;
    }
    self->muxSelect = 0;

    // Initialize params one by one
    ADC_init_param(&self->fundamental, POTENTIOMETER_50K);
    ADC_init_param(&self->fineTune, POTENTIOMETER_50K);
    ADC_init_param(&self->diffractionConstant, POTENTIOMETER_50K);
    ADC_init_param(&self->numOsc, POTENTIOMETER_50K);

    // Activate Mux pin
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET);

}

// TODO: Bitmask the GPIO
void _ADC_incrementMuxPins(adc_t *self){
    if (self->muxSelect){
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET);

        self->muxSelect = 0;
    }
    else {
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET);

        self->muxSelect = 1;
    }
}

void ADC_processBlock(adc_t *self)
{
    self->diffractionConstant.val = ((float)self->adcBuf[2] / ADC_BIT_DEPTH) * 5;
    self->numOsc.val = ((float)self->adcBuf[3] / ADC_BIT_DEPTH) * NUM_OSC;

    if (self->muxSelect){
        self->fundamental.val = (uint32_t)fbsmooth_process(&self->fundamental.fbsmooth, 0.999, self->adcBuf[0]);
    }
    else {
        self->fineTune.val = (uint32_t)fbsmooth_process(&self->fineTune.fbsmooth, 0.999, self->adcBuf[0]);
    }

    _ADC_incrementMuxPins(self);
}
