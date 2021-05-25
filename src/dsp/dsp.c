#include "dsp.h"
#include <arm_math.h>

void DSP_init(dsp_t *self)
{
    UX_init(&(self->uexkull), SAMPLE_RATE);
}

static void _DSP_processParams(dsp_t *self, adc_t *adc)
{
    float fundamental = ((float)adc->adc_params[FUNDAMENTAL_POT].val / ADC_BIT_DEPTH);
    float fine = ((float)adc->mux.params[FUNDAMENTAL_FINE_ATTENUVERTER].val / ADC_BIT_DEPTH);

    fundamental = (pow(fundamental, 2) * 15000.0f);
    fine = pow(fine, 2) * ((float)log(fundamental) * 50.0f);
    fundamental += fine;

    UX_calculateFrequencySeries(&self->uexkull,
        fundamental,
        adc->adc_params[DIFFRACTION_POT_1].val,
        0
    );

    UX_calculateFrequencySeries(&self->uexkull,
        fundamental,
        adc->adc_params[DIFFRACTION_POT_2].val,
        1
    );
}

void DSP_processBlock(dsp_t *self, adc_t *adc, bool isHalfCallback)
{
    int startBuf = isHalfCallback * BUF_SAMPLES / 2;
    int endBuf = startBuf + BUF_SAMPLES / 2;

    _DSP_processParams(self, adc);

    float gainArray[NUM_OSC];

    for (int i = 0; i < NUM_OSC; i++)
    {
        if (i < adc->adc_params[NUM_OSC_POT].val - 1){
            gainArray[i] = 0.3f;
        }
        // else if (i < adc->adc_params[NUM_OSC_POT].val && i + 1 > adc->adc_params[NUM_OSC_POT].val) {
            // gainArray[i] = 1.0f - adc->adc_params[NUM_OSC_POT].val;
        // }
        else {
            gainArray[i] = 0.0f;
        }
    }

    for (int pos = startBuf; pos < endBuf; pos += 4)
    {
        int lval = 0;
        int rval = 0;

        lval = UX_processLeftBank(&(self->uexkull), gainArray);
        rval = UX_processRightBank(&(self->uexkull), gainArray);

        lval <<= 16;
        rval <<= 16;

        self->txBuf[pos] = (lval >> 16) & 0xFFFF;
        self->txBuf[pos + 1] = lval & 0xFFFF;
        self->txBuf[pos + 2] = (rval >> 16) & 0xFFFF;
        self->txBuf[pos + 3] = rval & 0xFFFF;
    }
}