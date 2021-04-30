#include "dsp.h"

void DSP_init(dsp_t *self)
{
    UX_init(&(self->uexkull), SAMPLE_RATE);
}

static void DSP_processParams(dsp_t *self, adc_t *adc)
{
    UX_calculateFrequencySeries(&self->uexkull,
        adc->adcValue,
        0.5f
    );
}

void DSP_processBlock(dsp_t *self, adc_t *adc, bool isHalfCallback)
{
    int startBuf = isHalfCallback * BUF_SAMPLES / 2;
    int endBuf = startBuf + BUF_SAMPLES / 2;

    DSP_processParams(self, adc);

    for (int pos = startBuf; pos < endBuf; pos += 4)
    {
        int lval = 0;
        int rval = 0;

        // Convert to 32bit int range
        const float factor = (RAND_MAX / 2);

        lval = UX_process(&(self->uexkull)) * factor;
        rval = lval;

        self->txBuf[pos] = (lval >> 16) & 0xFFFF;
        self->txBuf[pos + 1] = lval & 0xFFFF;
        self->txBuf[pos + 2] = (rval >> 16) & 0xFFFF;
        self->txBuf[pos + 3] = rval & 0xFFFF;
    }
}