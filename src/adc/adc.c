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
    self->interSelector = 0;
    for (int i = 0; i < NUM_UX_ADC_PARAMS; i++)
    {
        dma_buf[i] = 0;
        if (i >= MUX_OFFSET)
            _ADC_init_adc_param(&self->adc_params[i], i);
        else
            _ADC_init_mux_param(&self->mux[i]);
    }

    for (int i = 0; i < NUM_UX_GPIO_PARAMS; i++)
    {
        _ADC_init_gpio_param(&self->gpio_params[i], i);
    }
}

static void _ADC_incrementMux(adc_t *self)
{
    if (self->interSelector == 7)
    {
        self->interSelector = 0;
    }
    else {
        self->interSelector++;
    }

    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, self->interSelector & 0x01);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, (self->interSelector >> 1) & 0x01);
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, (self->interSelector >> 2) & 0x01);

    for (int i = 0; i < NUM_UX_MUXS; i++)
    {
        self->mux[i].sel = self->interSelector;
    }
}

static void _ADC_processAttenuverterMux(mux_param_t *self)
{

    // First set the ADC param equal to the current mux
    // value from the DMA buffer
    adc_param_t *param = &self->params[self->sel];
    param->val = dma_buf[MUX_IN_ATTENUVERTERS];

    // Then, go one by one to process individual values and beautify
    // them: e.g. filtering, denoising, scaling
    switch (self->sel)
    {
    case (FUNDAMENTAL_FINE_ATTENUVERTER):
        param->val = (uint32_t)fbsmooth_process(&param->fbsmooth, 0.999, param->val);
        break;
    case (WAVEFORM_ATTENUVERTER):
        break;
    case (NUM_OSC_ATTENUVERTER):
        break;
    case (FM_ATTENUVERTER_1):
        break;
    case (FM_ATTENUVERTER_2):
        break;
    case (LFO_FREQ_ATTENUVERTER):
        break;
    case (LFO_PHASE_ATTENUVERTER):
        break;
    case (LFO_AMP_ATTENUVERTER):
        break;
    default:
        break;
    }
}

// adc_param_t *_ADC_processLEDs(mux_param_t *self)
// {
//     // First set the ADC param equal to the current mux
//     // value from the DMA buffer
//     adc_param_t param = self->params[self->sel];
//     param.val = dma_buf[MUX_IN_ATTENUVERTERS];

//     // Then, go one by one to process individual values and beautify
//     // them: e.g. filtering, denoising, scaling
//     switch (self->sel)
//     {
//     case (FUNDAMENTAL_FINE_ATTENUVERTER):
//         param.val =
//     case (WAVEFORM_ATTENUVERTER):
//     case (NUM_OSC_ATTENUVERTER):
//     case (FM_ATTENUVERTER_1):
//     case (FM_ATTENUVERTER_2):
//     case (LFO_FREQ_ATTENUVERTER):
//     case (LFO_PHASE_ATTENUVERTER):
//     case (LFO_AMP_ATTENUVERTER):
//     default:
//         break;
//     }

//     return param;
// }

static void _ADC_processPotMux(mux_param_t *self)
{
    // First set the ADC param equal to the current mux
    // value from the DMA buffer
    adc_param_t *param = &self->params[self->sel];
    param->val = dma_buf[MUX_IN_POTS];

    // Then, go one by one to process individual values and beautify
    // them: e.g. filtering, denoising, scaling
    switch (self->sel)
    {
    case (FUNDAMENTAL_POT):
        param->val = (uint32_t)fbsmooth_process(&param->fbsmooth, 0.999, param->val);
        break;
    case (DIFFRACTION_POT_1):
        param->val = ((float)param->val / ADC_BIT_DEPTH) * NUM_DIFFRACTION_CONSTANTS;
        break;
    case (DIFFRACTION_POT_2):
        param->val = ((float)param->val / ADC_BIT_DEPTH) * NUM_DIFFRACTION_CONSTANTS;
        break;
    case (WAVEFORM_POT):
        param->val = ((float)param->val / ADC_BIT_DEPTH) * NUM_WAVEFORMS;
        break;
    case (NUM_OSC_POT):
        param->val = ((float)param->val / ADC_BIT_DEPTH) * NUM_OSC;
        break;
    case (LFO_FREQ_POT):
        param->val = ((float)fbsmooth_process(&param->fbsmooth, 0.999, param->val) / ADC_BIT_DEPTH) * 5.0f;
        break;
    case (LFO_PHASE_POT):
        param->val = ((float)fbsmooth_process(&param->fbsmooth, 0.999, param->val) / ADC_BIT_DEPTH) * 5.0f;
        break;
    case (LFO_AMP_POT):
        param->val = (float)fbsmooth_process(&param->fbsmooth, 0.999, param->val);
        break;
    default:
        break;
    }
}

// adc_param_t _ADC_processADCValue(adc_param_t *self, uint8_t id)
// {
//     // First set the ADC param equal to the current mux
//     // value from the DMA buffer
//     adc_param_t *param = self;

//     if (id != MUX_IN_ATTENUVERTERS || id != MUX_IN_POTS)
//     {
//         switch (id)
//         {
//         case(FUNDAMENTAL_INPUT):
//             self->val =
//                 (uint32_t)fbsmooth_process(&self->fbsmooth,
//                     0.999,
//                     dma_buf[FUNDAMENTAL_INPUT]
//                 );
//             break;
//         case(WAVEFORM_INPUT):
//             break;
//         case(NUM_OSC_INPUT):
//             break;
//         case(LFO_FREQ_INPUT):
//             break;
//         case(LFO_PHASE_INPUT):
//             break;
//         case(LFO_AMP_INPUT):
//             break;
//         default:
//             break;
//         }
//     }

//     return *param;
// }

static void _ADC_processGPIOPins(adc_t *self)
{
    self->gpio_params[FUNDAMENTAL_LINLOG_SWITCH].val = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0);
    self->gpio_params[SPARSE_DENSE_SWITCH].val = HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1);
}

/**
 * Converts values from the raw dma_buf data to
 * the module-specific ADC parameters through smoothening,
 * scaling, and other functions. Everyone deserves a day at
 * the spa once in a while!
 */
void ADC_processBlock(adc_t *self)
{
    _ADC_processPotMux(&self->mux[UX_POT_MUX]);
    _ADC_processAttenuverterMux(&self->mux[UX_ATTENUVERTER_MUX]);
    _ADC_processGPIOPins(self);

    // Then process LEDS...

    // Then process audio inlets
    // for (int i = 0; i < NUM_UX_ADC_PARAMS; i++)
    // {
    //     self->adc_params[i] = _ADC_processADCValue(&self->adc_params[i], i);
    // }


    // Increment the mux select
    _ADC_incrementMux(self);
}
