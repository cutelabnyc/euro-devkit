#include "adc.h"
#include "uexkull.h"

#ifdef NO_POTS
static float HARDCODED_ATTENUVERTER_VALUES[NUM_ATTEN_MUX_IDS] = {
    0, // Fine Tune Freq
    0, // Waveform
    0, // Number of Oscillators
    1000, // FM
    1000, // AM
    0, // LFO Freq
    0, // LFO Phase
    0 // LFO Amp
};

static float HARDCODED_POT_VALUES[NUM_POT_MUX_IDS] = {
    100, // Fundamental
    1, // Diffraction Const Bank 1 (0, 1, 2, 3, 4)
    2, // Diffraction Const Bank 2 (0, 1, 2, 3, 4)
    0, // Waveform (0, 1, 2, 3)
    3, // Number of Oscillators (0 -10)
    1000, // LFO Freq (0 - 4096)
    3000, // LFO Phase (0 - 4096)
    1000 // LFO Amp (0 - 4096)
};
#endif

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
    for (int i = 0; i < NUM_ADC_IDS; i++)
    {
        dma_buf[i] = 0;
        if (i >= MUX_OFFSET)
            _ADC_init_adc_param(&self->adc_params[i], i);
        else
            _ADC_init_mux_param(&self->mux[i]);
    }

    for (int i = 0; i < NUM_GPIO_IDS; i++)
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

    for (int i = 0; i < NUM_MUX_IDS; i++)
    {
        self->mux[i].sel = self->interSelector;
    }
}

// NOTE: It would be super usefull in the future to do all of
// your smoothening and processing of values from hardware here
// and allow the data returned to be of a mutable type so that 
// values can play directly into the DSP routine without type casting!
// Also maybe write some sort of interface for this function 
static void _ADC_processAttenuverterMux(mux_param_t *self)
{

    // First set the ADC param equal to the current mux
    // value from the DMA buffer
    adc_param_t *param = &self->params[self->sel];
    param->val = dma_buf[MUX_IN_ATTENUVERTERS];

#ifdef NO_POTS
    param->val = HARDCODED_ATTENUVERTER_VALUES[self->sel];
    return;
#endif

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
    case (FM_ATTENUVERTER):
        param->val = (uint32_t)fbsmooth_process(&param->fbsmooth, 0.999, param->val);
        break;
    case (AM_ATTENUVERTER):
        param->val = (uint32_t)fbsmooth_process(&param->fbsmooth, 0.999, param->val);
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

// NOTE: It would be super usefull in the future to do all of
// your smoothening and processing of values from hardware here
// and allow the data returned to be of a mutable type so that 
// values can play directly into the DSP routine without type casting!
// Also maybe write some sort of interface for this function 
static void _ADC_processPotMux(mux_param_t *self)
{
    // First set the ADC param equal to the current mux
    // value from the DMA buffer
    adc_param_t *param = &self->params[self->sel];
    param->val = dma_buf[MUX_IN_POTS];

#ifdef NO_POTS
    param->val = HARDCODED_POT_VALUES[self->sel];
    return;
#endif

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
        param->val = (uint32_t)fbsmooth_process(&param->fbsmooth, 0.999, param->val);
        break;
    case (LFO_FREQ_POT):
        param->val = (uint32_t)fbsmooth_process(&param->fbsmooth, 0.999, param->val);
        break;
    case (LFO_PHASE_POT):
        param->val = (uint32_t)fbsmooth_process(&param->fbsmooth, 0.999, param->val);
        break;
    case (LFO_AMP_POT):
        param->val = (uint32_t)fbsmooth_process(&param->fbsmooth, 0.999, param->val);
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
    // for (int i = 0; i < NUM_ADC_IDS; i++)
    // {
    //     self->adc_params[i] = _ADC_processADCValue(&self->adc_params[i], i);
    // }


    // Increment the mux select
    _ADC_incrementMux(self);
}
