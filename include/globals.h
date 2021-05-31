#ifndef __GLOBALS_H
#define __GLOBALS_H

#ifdef __cplusplus
extern "C"
{
#endif

#define NUM_UX_ADC_PARAMS 8
#define NUM_UX_GPIO_PARAMS 2
#define NUM_UX_MUX_PARAMS 8
#define NUM_UX_MUXS 2
#define MUX_OFFSET NUM_UX_MUXS
#define NUM_UX_LED_PARAMS 10

#include <cuteop.h>

    const enum  {
        MUX_IN_POTS,
        MUX_IN_ATTENUVERTERS,
        FUNDAMENTAL_INPUT,
        WAVEFORM_INPUT,
        NUM_OSC_INPUT,
        LFO_FREQ_INPUT,
        LFO_PHASE_INPUT,
        LFO_AMP_INPUT,
    } UX_ADC_IDs;

    const enum {
        PA3,
        PA5,
        PA6,
        PB0,
        PB1,
        PC0,
        PC2,
        // add one more pin!
    } UX_PIN_IDs;

    const enum  {
        FUNDAMENTAL_LINLOG_SWITCH,
        SPARSE_DENSE_SWITCH
    } UX_GPIO_IDs;

    const enum {
        UX_ATTENUVERTER_MUX,
        UX_POT_MUX,
    } UX_MUX_IDs;

    const enum {
        FUNDAMENTAL_FINE_ATTENUVERTER,
        WAVEFORM_ATTENUVERTER,
        NUM_OSC_ATTENUVERTER,
        FM_ATTENUVERTER_1,
        FM_ATTENUVERTER_2,
        LFO_FREQ_ATTENUVERTER,
        LFO_PHASE_ATTENUVERTER,
        LFO_AMP_ATTENUVERTER
    } UX_ATTEN_MUX_IDs;

    const enum {
        FUNDAMENTAL_POT,
        DIFFRACTION_POT_1,
        DIFFRACTION_POT_2,
        WAVEFORM_POT,
        NUM_OSC_POT,
        LFO_FREQ_POT,
        LFO_PHASE_POT,
        LFO_AMP_POT,
    } UX_POT_MUX_IDs;

    const enum {
        FUNDAMENTAL_LED,
        DIFFRACTION_LED_1,
        DIFFRACTION_LED_2,
        WAVEFORM_LED,
        NUM_OSC_LED,
        FM_LED_1,
        FM_LED_2,
        LFO_FREQ_LED,
        LFO_PHASE_LED,
        LFO_AMP_LED
    } UX_LED_IDs;

#ifdef __cplusplus
}
#endif

#endif /* __GLOBALS_H */
