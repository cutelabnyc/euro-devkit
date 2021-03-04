#include "uexkull.h"

void UX_init(uexkull_t *self, float samplerate)
{
    for (int i = 0; i < NUM_BANKS; i++)
    {
        BK_init(&(self->centralBanks[i]),
                NUM_OSC,
                samplerate,
                440.0f,
                0.5f,
                SIN);

        // NOTE: This'll be for LFOs
        // BK_init(&(self->reactiveBanks[i]),
        //         NUM_OSC,
        //         samplerate,
        //         440.0f,
        //         0.5f,
        //         SIN);
    }
}

void getFreqVector(uexkull_t *self, float fundamental, float mult)
{
    self->freqs[0] = fundamental;

    for (int i = 1; i < NUM_OSC; i++)
    {
        self->freqs[i] = (self->freqs[i - 1] + (self->freqs[i - 1] * mult));

        if (self->freqs[i] > MAX_FREQ)
        {
            self->freqs[i] = 0;
        }
    }
}

float UX_process(uexkull_t *self, float mult, float freq)
{
    float sig = 0;
    getFreqVector(self, freq, mult);

    for (int i = 0; i < NUM_BANKS; i++)
    {
        BK_setFrequencyVectors(&(self->centralBanks[i]), self->freqs, NUM_OSC);

        sig += (BK_process(&(self->centralBanks[i])) / NUM_BANKS);
    }

    return sig;
}