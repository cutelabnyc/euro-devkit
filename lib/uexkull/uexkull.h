// /**
//  * uexkull.h —— (Max Ardito, 10/07/20)
//  *
//  * Class representing the entire module's functionality.
//  * Instantiated globally in [/main.cpp]. Bridges the gap
//  * between the Daisy's Oscillator classes and Cute-Op's
//  * sequence class. Processes frequency ratios in the main
//  * loop
//  */

// #ifndef UEXKULL_H
// #define UEXKULL_H

// #define NUM_BANKS 1
// #define NUM_OSC 4
// #define MAX_FREQ 20000

// #include <cuteop.h>

//  /**
//   * uexkull_t: Module's main data structure
//   */
// typedef struct uexkull
// {
//     bank_t centralBanks[NUM_BANKS];
//     float freqs[NUM_OSC];
//     //sequence_t f;
// } uexkull_t;

// /**
//  * Initialize the 'uexkull' struct
//  */
// void UX_init(uexkull_t *self,
//     float samplerate
// /*t_sequence f*/);

// /**
//  * Frees the 'uexkull' struct
//  *
//  * TODO: Add and describe parameters
//  */
// void UX_destroy(uexkull_t *self);

// /**
//  * Set fundamental frequency
//  */
// void UX_setFreq(uexkull_t *self, float freq);

// /**
//  * Processes a single sample in the module's IO. The process
//  * function acts as a bridge between the Daisy's DSP library
//  * and Cute-Op's mathematical sequence generating module.
//  *
//  * TODO: Add and describe parameters
//  */
// float UX_process(uexkull_t *self);

// #endif /* OPPORTUNITY_H */
