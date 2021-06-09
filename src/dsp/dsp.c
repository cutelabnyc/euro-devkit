#include "dsp.h"
#include <math.h>

void DSP_init(dsp_t *self)
{
    UX_init(&(self->uexkull), SAMPLE_RATE);
}

static void _DSP_processParams(dsp_t *self, adc_t *adc)
{
    float fundamental = ((float)adc->mux[UX_POT_MUX].params[FUNDAMENTAL_POT].val / ADC_BIT_DEPTH);
    float fine = ((float)adc->mux[UX_ATTENUVERTER_MUX].params[FUNDAMENTAL_FINE_ATTENUVERTER].val / ADC_BIT_DEPTH);

    float lfoFreq = (float)(adc->mux[UX_POT_MUX].params[LFO_FREQ_POT].val / ADC_BIT_DEPTH) * 2.0f;
    float lfoPhase = (float)(adc->mux[UX_POT_MUX].params[LFO_PHASE_POT].val / ADC_BIT_DEPTH) * 2.0f;
    float lfoAmp = (float)(adc->mux[UX_POT_MUX].params[LFO_AMP_POT].val / ADC_BIT_DEPTH);

    if (adc->gpio_params[FUNDAMENTAL_LINLOG_SWITCH].val)
    {
        fundamental = ((float)(fundamental * fundamental) * MAX_FREQ);
        fine = (fine * fine) * ((float)log(fundamental) * 50.0f);
    }
    else {
        fundamental = ((float)fundamental * MAX_FREQ);
        fine = fine * ((float)log(fundamental) * 50.0f);
    }
    fundamental += fine;

    UX_setWaveform(&self->uexkull, adc->mux[UX_POT_MUX].params[WAVEFORM_POT].val);
    UX_calculateLFOFrequencies(&self->uexkull, lfoFreq, lfoPhase, lfoAmp);

    UX_calculateFrequencySeries(&self->uexkull,
        fundamental,
        adc->mux[UX_POT_MUX].params[DIFFRACTION_POT_1].val,
        0,
        adc->gpio_params[SPARSE_DENSE_SWITCH].val
    );

    UX_calculateFrequencySeries(&self->uexkull,
        fundamental,
        adc->mux[UX_POT_MUX].params[DIFFRACTION_POT_2].val,
        1,
        adc->gpio_params[SPARSE_DENSE_SWITCH].val
    );
}

void DSP_processBlock(dsp_t *self, adc_t *adc, bool isHalfCallback)
{
    int startBuf = isHalfCallback * BUF_SAMPLES / 2;
    int endBuf = startBuf + BUF_SAMPLES / 2;

    _DSP_processParams(self, adc);

    float gainCurve[NUM_OSC];

    for (int i = 0; i < NUM_OSC; i++)
    {
        // TODO: Smoothen/Log out these values as a curve for number of oscillators

#ifdef NO_POTS
        gainCurve[i] = 2;
#else
        gainCurve[i] = ((float)adc->mux[UX_POT_MUX].params[NUM_OSC_POT].val / ADC_BIT_DEPTH);
#endif
    }

    for (int pos = startBuf; pos < endBuf; pos += 4)
    {
        int lval = 0;
        int rval = 0;

        lval = UX_processLeftBank(&(self->uexkull), gainCurve);
        rval = UX_processRightBank(&(self->uexkull), gainCurve);

        lval <<= 15;
        rval <<= 15;

        self->txBuf[pos] = (lval >> 16) & 0xFFFF;
        self->txBuf[pos + 1] = lval & 0xFFFF;
        self->txBuf[pos + 2] = (rval >> 16) & 0xFFFF;
        self->txBuf[pos + 3] = rval & 0xFFFF;
    }
}