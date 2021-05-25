#include "adc.h"
#include "uexkull.h"

static void _ADC_init_adc_param(adc_param_t *self, uint8_t id)
{
    self->val = 0;
    self->id = id;
    fbsmooth_init(&self->fbsmooth);
}

static void _ADC_init_gpio_param(gpio_param_t *self, uint8_t id)
{
    self->id = id;
    self->val = 0;
}

static void _ADC_init_mux_param(mux_param_t *self)
{
    self->sel = 0;
    for (int i = 0; i < NUM_UX_MUX_PARAMS; i++)
    {
        _ADC_init_adc_param(&self->params[i], i);
    }
}

void ADC_init(adc_t *self, ADC_HandleTypeDef *adcx)
{
    // Initialize HAL ADC and ADC DMA buffer
    self->hadc = adcx;

    for (int i = 0; i < NUM_UX_ADC_PARAMS; i++)
    {
        dma_buf[i] = 0;
        _ADC_init_adc_param(&self->adc_params[i], i);
    }

    for (int i = 0; i < NUM_UX_GPIO_PARAMS; i++)
    {
        _ADC_init_gpio_param(&self->gpio_params[i], i);
    }

    _ADC_init_mux_param(&self->mux);
}

// TODO: Bitmask the GPIO
// void _ADC_incrementMuxPins(adc_t *self){
//     if (self->muxSelect){
//         HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
//         HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);
//         HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET);
//         HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET);

//         self->muxSelect = 0;
//     }
//     else {
//         HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);
//         HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);
//         HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET);
//         HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET);

//         self->muxSelect = 1;
//     }
// }

/**
 * Converts values from the raw dma_buf data to
 * the module-specific ADC parameters through smoothening,
 * scaling, and other functions. Everyone deserves a day at
 * the spa once in a while!
 */
void ADC_processBlock(adc_t *self)
{

    // Switch statement for beautifying whichever mux parameter is selected
    // if (self->muxSelect){
    //     self->fundamental.val = (uint32_t)fbsmooth_process(&self->fundamental.fbsmooth, 0.999, self->dma_buf[0]);
    // }
    // else {
    //     self->fineTune.val = (uint32_t)fbsmooth_process(&self->fineTune.fbsmooth, 0.999, self->dma_buf[0]);
    // }

    // Beautify the remaining parameters
    self->adc_params[FUNDAMENTAL_POT].val = (uint32_t)fbsmooth_process(&self->adc_params[FUNDAMENTAL_POT].fbsmooth, 0.999, dma_buf[FUNDAMENTAL_POT]);
    // self->fineTune.val = (uint32_t)fbsmooth_process(&self->fineTune.fbsmooth, 0.999, self->dma_buf[0]);
    self->adc_params[DIFFRACTION_POT_1].val = ((float)dma_buf[DIFFRACTION_POT_1] / ADC_BIT_DEPTH) * 5;
    self->adc_params[NUM_OSC_POT].val = ((float)dma_buf[NUM_OSC_POT] / ADC_BIT_DEPTH) * NUM_OSC;

    // Increment the mux select
    // _ADC_incrementMux(&self);
}
