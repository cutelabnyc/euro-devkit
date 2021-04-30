#ifndef __CODEC_H
#define __CODEC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include "stm32f7xx_hal.h"

#define W8731_ADDR_0 0x1A
#define W8731_ADDR_1 0x1B
#define W8731_NUM_REGS 10

#define CODEC_ADDRESS                   (W8731_ADDR_0 << 1)
#define CODEC_FLAG_TIMEOUT             ((uint32_t)0x1000)

    void Codec_Init(I2C_HandleTypeDef *hi2c);


#ifdef __cplusplus
}
#endif

#endif /* __CODEC_H */
