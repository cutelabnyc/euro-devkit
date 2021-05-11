#include "dsp.h"

void DSP_init(dsp_t *self)
{
    UX_init(&(self->uexkull), SAMPLE_RATE);
}

static void DSP_processParams(dsp_t *self, adc_t *adc)
{
    float f = ((float)adc->fundamental.val / 256.0f);
    f = f * f * 15000.0f;
    // uint8_t diffractionConstant = adc->diffractionConstant.val
    UX_calculateFrequencySeries(&self->uexkull,
        f,
        adc->diffractionConstant.val,
        0
    );

    UX_calculateFrequencySeries(&self->uexkull,
        f,
        adc->diffractionConstant.val,
        1
    );
}

void DSP_processBlock(dsp_t *self, adc_t *adc, bool isHalfCallback)
{
    int startBuf = isHalfCallback * BUF_SAMPLES / 2;
    int endBuf = startBuf + BUF_SAMPLES / 2;

    DSP_processParams(self, adc);

    float gainArray[NUM_OSC];

    for (int i = 0; i < NUM_OSC; i++)
    {
        if (i < adc->numOsc.val - 1){
            gainArray[i] = 1.0f;
        }
        else if (i < adc->numOsc.val && i + 1 > adc->numOsc.val) {
            gainArray[i] = adc->numOsc.val % 1;
        }
        else {
            gainArray[i] = 0.0f;

        }
    }

    for (int pos = startBuf; pos < endBuf; pos += 4)
    {
        int lval = 0;
        int rval = 0;

        // Convert to 32bit int range
        const float factor = (RAND_MAX / 2);

        lval = UX_processLeftBank(&(self->uexkull), gainArray) * factor;
        rval = UX_processRightBank(&(self->uexkull), gainArray) * factor;

        self->txBuf[pos] = (lval >> 16) & 0xFFFF;
        self->txBuf[pos + 1] = lval & 0xFFFF;
        self->txBuf[pos + 2] = (rval >> 16) & 0xFFFF;
        self->txBuf[pos + 3] = rval & 0xFFFF;
    }
}