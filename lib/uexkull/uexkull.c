#include "uexkull.h"
#include <cuteop.h>

/**
* _UX_diffractionSeries:
*
* This is the bread and butter of Uexkull, the heart and soul!
*/
static void _UX_diffractionSeries(float *vector, uint16_t numElements, float diffractionConstant)
{
    for (int i = 0; i < numElements; i++)
    {
        if (i != 0)
        {
            vector[i] = vector[i - 1] + (vector[i - 1] * diffractionConstant);
        }
    }
}

void UX_init(uexkull_t *self, float samplerate)
{
    bank_init(&self->bank,
        NUM_OSC,
        samplerate,
        0.0f,
        SIN);

    // series_init(
    //     &self->series,
    //     NUM_OSC,
    //     _UX_diffractionSeries
    // );

    self->_diffractionConstant = 0.5f;
    self->_fundamental = 0;
    self->_diffractionWidth = false;
    // series_process(&self->series, self->_fundamental, &self->_diffractionConstant);

}

void UX_calculateFrequencySeries(uexkull_t *self, float fundamental, float diffractionConstant)
{
    self->_fundamental = fundamental;
    self->_diffractionConstant = diffractionConstant;
    self->freqArray[0] = fundamental;
    _UX_diffractionSeries(self->freqArray, NUM_OSC, self->_diffractionConstant);
    // TODO: Use the series class in cuteop
    // series_process(&self->series, self->_fundamental, 1, &self->_diffractionConstant);
}

float UX_process(uexkull_t *self)
{
    float sig = 0;
    bank_setFrequencies(&(self->bank), self->freqArray, NUM_OSC);
    sig += bank_process(&(self->bank));

    return sig;
}