/**
 * uexkull.h —— (Max Ardito, 10/07/20)
 *
 * Class representing the entire module's functionality.
 * Instantiated globally in [/main.cpp]. Bridges the gap
 * between the Daisy's Oscillator classes and Cute-Op's
 * sequence class. Processes frequency ratios in the main
 * loop
 */

#ifndef UEXKULL_H
#define UEXKULL_H

#define NUM_OSC 5
#define NUM_BANKS 2
#define MAX_FREQ 20000
#define NUM_DIFFRACTION_CONSTANTS 5
#define NUM_WAVEFORMS 4

#include <cuteop.h>

 /**
 * uexkull_t: Module's main data structure
 */
typedef struct uexkull
{
    t_bank bank[NUM_BANKS];
    t_bank lfo[NUM_BANKS];
    float freqArray[NUM_BANKS][NUM_OSC];
    float lfoFreqArray[NUM_BANKS][NUM_OSC];
    float _diffractionConstant[NUM_BANKS];
    bool _diffractionWidth[NUM_BANKS]; //0: sparse, 1: dense
    float _fundamental;
} uexkull_t;

/**
 * Constants for diffraction series calculation
 */
static const float diffractionConstants[NUM_DIFFRACTION_CONSTANTS] =
{
    1.0f,
    0.5f,
    0.333f,
    0.2f,
    0.142857f
};

/**
 * Initialize the 'uexkull' struct
 */
void UX_init(uexkull_t *self, float samplerate);

/**
 * Frees the 'uexkull' struct
 */
void UX_destroy(uexkull_t *self);

/**
 * Calculates the frequency series based on the fundamental
 */
void UX_calculateFrequencySeries(uexkull_t *self, float fundamental, uint8_t numConstant, uint8_t numBank);

/**
 * Set waveform on both banks
 */
void UX_setWaveform(uexkull_t *self, waveform_t waveform);

/**
 * Processes a single sample in the module's IO. The process
 * function acts as a bridge between the Daisy's DSP library
 * and Cute-Op's mathematical sequence generating module.
 */
float UX_processLeftBank(uexkull_t *self, float *gainValues);
float UX_processRightBank(uexkull_t *self, float *gainValues);


#endif /* OPPORTUNITY_H */
