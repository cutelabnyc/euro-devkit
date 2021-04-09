#include "uexkull.h"
#include <cuteop.h>

void UX_init(uexkull_t *self, float samplerate)
{
    bank_init(&(self->bank),
        NUM_OSC,
        samplerate,
        0.0f,
        SIN);

    for (int i = 0; i < NUM_OSC; i++)
    {
        self->freqs[i] = 0;
    }
}

void UX_getFreqVector(uexkull_t *self, float mult)
{
    for (int i = 1; i < NUM_OSC; i++)
    {
        self->freqs[i] = (self->freqs[i - 1] + (self->freqs[i - 1] * mult));

        if (self->freqs[i] > MAX_FREQ)
        {
            self->freqs[i] = 0;
        }
    }
}

void UX_setFreq(uexkull_t *self, float freq)
{
    self->freqs[0] = freq;
}

float UX_process(uexkull_t *self)
{
    float sig = 0;

    bank_setFrequencies(&(self->bank), self->freqs, NUM_OSC);
    sig += bank_process(&(self->bank));

    return sig;
}