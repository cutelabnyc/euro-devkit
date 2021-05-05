#include "adc.h"


void ADC_init(adc_t *self, ADC_HandleTypeDef *adcx)
{
    self->hadc = adcx;
    self->adcValArray[0] = 0;
    self->adcValArray[1] = 0;


    rampsmooth_init(&self->rampsmooth, 4);
}


void ADC_processBlock(adc_t *self)
{
    // HAL_ADC_Start(self->hadc);
    // if (HAL_ADC_PollForConversion(self->hadc, 5) == HAL_OK)
    // {
    //     self->adcValue = HAL_ADC_GetValue(self->hadc);
    // }
    // HAL_ADC_Stop(self->hadc);
}
