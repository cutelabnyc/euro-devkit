#include "uexkull.h"

void UX_init(uexkull_t *self, float samplerate)
{
    for (int i = 0; i < NUM_BANKS; i++)
    {
        BK_init(&(self->centralBanks[i]),
                NUM_OSC,
                samplerate,
                0.0f,
                0.0f,
                SIN);
    }

    for (int i = 0; i < NUM_OSC; i++)
    {
        self->freqs[i] = 0;
    }
}

static void _UX_getFreqVector(uexkull_t *self, float mult)
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

    _UX_getFreqVector(self, 0.5);

    for (int i = 0; i < NUM_BANKS; i++)
    {
        // BK_setFrequencyVectors(&(self->centralBanks[i]), self->freqs, NUM_OSC);

        sig += (BK_process(&(self->centralBanks[i]), self->freqs)) / NUM_BANKS;
    }

    return sig;
}