#include "dsp.h"
#include <arm_math.h>

void DSP_init(dsp_t *self)
{
    UX_init(&(self->uexkull), SAMPLE_RATE);
}

static void _DSP_processParams(dsp_t *self, adc_t *adc)
{
    float fundamental = ((float)adc->mux[UX_POT_MUX].params[FUNDAMENTAL_POT].val / ADC_BIT_DEPTH);
    float fine = ((float)adc->mux[UX_ATTENUVERTER_MUX].params[FUNDAMENTAL_FINE_ATTENUVERTER].val / ADC_BIT_DEPTH);

    fundamental = ((fundamental * fundamental) * 15000.0f);
    fine = (fine * fine) * ((float)log(fundamental) * 50.0f);
    fundamental += fine;

    UX_setWaveform(&self->uexkull, adc->mux[UX_POT_MUX].params[WAVEFORM_POT].val);

    UX_calculateFrequencySeries(&self->uexkull,
        fundamental,
        adc->mux[UX_POT_MUX].params[DIFFRACTION_POT_1].val,
        0
    );

    UX_calculateFrequencySeries(&self->uexkull,
        fundamental,
        adc->mux[UX_POT_MUX].params[DIFFRACTION_POT_2].val,
        1
    );

    UX_calculateLFOFrequencies(&self->uexkull,
        adc->mux[UX_POT_MUX].params[LFO_FREQ_POT].val,
        adc->mux[UX_POT_MUX].params[LFO_PHASE_POT].val,
        adc->mux[UX_POT_MUX].params[LFO_AMP_POT].val
    );
    // UX_setNumOscillators(&self->uexkull, adc->mux[UX_POT_MUX].params[NUM_OSC_POT].val);
}

void DSP_processBlock(dsp_t *self, adc_t *adc, bool isHalfCallback)
{
    int startBuf = isHalfCallback * BUF_SAMPLES / 2;
    int endBuf = startBuf + BUF_SAMPLES / 2;

    _DSP_processParams(self, adc);

    for (int pos = startBuf; pos < endBuf; pos += 4)
    {
        int lval = 0;
        int rval = 0;

        lval = UX_processLeftBank(&(self->uexkull));
        rval = UX_processRightBank(&(self->uexkull));

        lval <<= 15;
        rval <<= 15;

        self->txBuf[pos] = (lval >> 16) & 0xFFFF;
        self->txBuf[pos + 1] = lval & 0xFFFF;
        self->txBuf[pos + 2] = (rval >> 16) & 0xFFFF;
        self->txBuf[pos + 3] = rval & 0xFFFF;
    }
}