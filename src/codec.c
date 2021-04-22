#include "codec.h"

/* local vars */
__IO uint32_t  CODECTimeout = CODEC_LONG_TIMEOUT;
__IO uint8_t OutputDev = 0;

const uint16_t w8731_init_data[] =
{
    0x017,			// Reg 00: Left Line In (0dB, mute off)
    0x017,			// Reg 01: Right Line In (0dB, mute off)
    0x07F,			// Reg 02: Left Headphone out (0dB)
    0x07F,			// Reg 03: Right Headphone out (0dB)
    0x012,			// Reg 04: Analog Audio Path Control (DAC sel, Mute Mic)
    0x000,			// Reg 05: Digital Audio Path Control
    0x062,			// Reg 06: Power Down Control (Clkout, Osc, Mic Off)
    0x00E,			// Reg 07: Digital Audio Interface Format (i2s, 32-bit, slave)
    0x256,			// Reg 08: Sampling Control (Normal, 256x, 48k ADC/DAC)
    0x001			// Reg 09: Active Control
};


/**
  * @brief  Resets the audio codec. It restores the default configuration of the
  *         codec (this function shall be called before initializing the codec).
  * @note   This function calls an external driver function: The IO Expander driver.
  * @param  None
  * @retval None
  */
void Codec_Init(I2C_HandleTypeDef *hi2c)
{
    uint8_t i;

    /* Load default values */
    for (i = 0; i < W8731_NUM_REGS; i++)
    {
        /* Assemble 2-byte data in WM8731 format */
        uint8_t yamAndCheese[2];
        yamAndCheese[0] = ((i << 1) & 0xFE) | ((w8731_init_data[i] >> 8) & 0x01);
        yamAndCheese[1] = w8731_init_data[i] & 0xFF;

        HAL_I2C_Master_Transmit(hi2c, (W8731_ADDR_0 << 1), (uint8_t *)yamAndCheese, sizeof(uint16_t), CODEC_FLAG_TIMEOUT);
    }

}