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
    for (int i = 0; i < NUM_BANKS; i++)
    {
        bank_init(&self->bank[i],
            NUM_OSC,
            samplerate,
            0.0f,
            SIN);

        for (int j = 0; j < NUM_OSC; j++)
        {
            self->freqArray[i][j] = 0.0f;
        }
        self->_diffractionConstant[i] = diffractionConstants[0];
        self->_diffractionWidth[i] = false;
    }

    self->_fundamental = 0;

}

void UX_setWaveform(uexkull_t *self, waveform_t waveform)
{
    for (int i = 0; i < NUM_BANKS; i++)
    {
        bank_setWaveform(&self->bank[i], waveform);
    }
}

void UX_calculateFrequencySeries(uexkull_t *self, float fundamental, uint8_t numConstant, uint8_t numBank)
{
    self->_fundamental = fundamental;

    self->_diffractionConstant[numBank] = diffractionConstants[numConstant];
    self->freqArray[numBank][0] = fundamental;
    _UX_diffractionSeries(self->freqArray[numBank], NUM_OSC, self->_diffractionConstant[numBank]);
}

float UX_processLeftBank(uexkull_t *self, float *gainValues)
{
    float sig = 0;
    bank_setFrequencies(&(self->bank[0]), self->freqArray[0], NUM_OSC);
    sig += bank_process(&(self->bank[0]), gainValues);

    return sig;
}

float UX_processRightBank(uexkull_t *self, float *gainValues)
{
    float sig = 0;
    bank_setFrequencies(&(self->bank[1]), self->freqArray[1], NUM_OSC);
    sig += bank_process(&(self->bank[1]), gainValues);

    return sig;
}
