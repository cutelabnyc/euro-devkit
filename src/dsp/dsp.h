#ifndef __DSP_H
#define __DSP_H

#ifdef __cplusplus
extern "C"
{
#endif

#define SAMPLES 512             // Total number of samples left and right
#define BUF_SAMPLES SAMPLES * 4 // Size of DMA tx/rx buffer samples * left/right * 2 for 32 bit samples
#define SAMPLE_RATE 16000

#include <stdbool.h>
#include <stdlib.h>
#include "stm32f7xx_hal.h"
#include "uexkull.h"
#include "../adc/adc.h"

    typedef struct dsp {
        uint16_t rxBuf[BUF_SAMPLES];
        uint16_t txBuf[BUF_SAMPLES];
        uexkull_t uexkull;
    } dsp_t;

    void DSP_init(dsp_t *self);
    void DSP_processBlock(dsp_t *self, adc_t *adc, bool isHalfCallback);

#ifdef __cplusplus
}
#endif

#endif /* __DSP_H */
