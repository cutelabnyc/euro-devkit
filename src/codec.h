#ifndef __CODEC_H
#define __CODEC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include "stm32f7xx_hal.h"

    /* Maximum Timeout values for flags and events waiting loops. These timeouts are
       not based on accurate values, they just guarantee that the application will
       not remain stuck if the I2C communication is corrupted.
       You may modify these timeout values depending on CPU frequency and application
       conditions (interrupts routines ...). */
#define CODEC_FLAG_TIMEOUT             ((uint32_t)0x1000)
#define CODEC_LONG_TIMEOUT             ((uint32_t)(300 * CODEC_FLAG_TIMEOUT))

    void Codec_Reset(void);
    uint32_t Codec_WriteRegister(uint8_t RegisterAddr, uint16_t RegisterValue);


#ifdef __cplusplus
}
#endif

#endif /* __CODEC_H */
