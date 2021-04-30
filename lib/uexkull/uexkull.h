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

#define NUM_OSC 10
#define NUM_BANKS 2
#define MAX_FREQ 20000
#define NUM_DIFFRACTION_CONSTANTS 5

#include <cuteop.h>

 /**
 * uexkull_t: Module's main data structure
 */
typedef struct uexkull
{
    t_bank bank;
    // t_series series;
    float freqArray[NUM_OSC];
    float _fundamental;
    float _diffractionConstant;
    bool _diffractionWidth; //0: sparse, 1: dense
} uexkull_t;

/**
 * Constants for diffraction series calculation
 */
 // const float diffractionConstants[NUM_DIFFRACTION_CONSTANTS] =
 // {
 //     1.0f,
 //     0.5f,
 //     0.3f,
 //     0.2f,
 //     0.142857f
 // };

 /**
  * Initialize the 'uexkull' struct
  */
void UX_init(uexkull_t *self, float samplerate);

/**
 * Frees the 'uexkull' struct
 *
 * TODO: Add and describe parameters
 */
void UX_destroy(uexkull_t *self);

/**
 * Calculates the frequency series based on the fundamental
 */
void UX_calculateFrequencySeries(uexkull_t *self, float fundamental, float diffractionConstant);

/**
 * Processes a single sample in the module's IO. The process
 * function acts as a bridge between the Daisy's DSP library
 * and Cute-Op's mathematical sequence generating module.
 *
 * TODO: Add and describe parameters
 */
float UX_process(uexkull_t *self);

#endif /* OPPORTUNITY_H */
