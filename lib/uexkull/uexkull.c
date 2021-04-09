#include "uexkull.h"
#include <cuteop.h>

/**
* _UX_diffractionSeries:
*
* This is the bread and butter of Uexkull, the heart and soul!
*/
static void _UX_diffractionSeries(float *vector, uint16_t numElements)
{
    for (int i = 0; i < numElements; i++)
    {
        if (i != 0)
        {
            vector[i] = vector[i - 1] + (vector[i - 1] * 0.5f);
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

    series_init(
        &self->series,
        NUM_OSC,
        _UX_diffractionSeries
    );
}

void UX_calculateFrequencySeries(uexkull_t *self, float fundamental)
{
    self->_fundamental = fundamental;
    series_process(&self->series, self->_fundamental);
}

float UX_process(uexkull_t *self)
{
    float sig = 0;
    bank_setFrequencies(&(self->bank), series_getSeries(&self->series), NUM_OSC);
    sig += bank_process(&(self->bank));

    return sig;
}