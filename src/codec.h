#ifndef __CODEC_H
#define __CODEC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include "stm32f7xx_hal.h"

    /*Mask for the bit EN of the I2S CFGR register */
#define I2S_ENABLE_MASK                 0x0400

        /* Codec audio Standards */
#ifdef I2S_STANDARD_PHILLIPS
#define  CODEC_STANDARD                0x04
#define I2S_STANDARD                   I2S_Standard_Phillips         
#elif defined(I2S_STANDARD_MSB)
#define  CODEC_STANDARD                0x00
#define I2S_STANDARD                   I2S_Standard_MSB    
#elif defined(I2S_STANDARD_LSB)
#define  CODEC_STANDARD                0x08
#define I2S_STANDARD                   I2S_Standard_LSB    
#else 
#error "Error: No audio communication standard selected !"
#endif /* I2S_STANDARD */

#define W8731_ADDR_0 0x1A
#define W8731_ADDR_1 0x1B
#define W8731_NUM_REGS 10

    /* The 7 bits Codec address (sent through I2C interface) */
#define CODEC_ADDRESS           (W8731_ADDR_0<<1)

    /* Maximum Timeout values for flags and events waiting loops. These timeouts are
       not based on accurate values, they just guarantee that the application will
       not remain stuck if the I2C communication is corrupted.
       You may modify these timeout values depending on CPU frequency and application
       conditions (interrupts routines ...). */
#define CODEC_FLAG_TIMEOUT             ((uint32_t)0x1000)
#define CODEC_LONG_TIMEOUT             ((uint32_t)(300 * CODEC_FLAG_TIMEOUT))

    void Codec_Init(I2C_HandleTypeDef *hi2c);
    // uint32_t Codec_WriteRegister(uint8_t RegisterAddr, uint16_t RegisterValue);


#ifdef __cplusplus
}
#endif

#endif /* __CODEC_H */
