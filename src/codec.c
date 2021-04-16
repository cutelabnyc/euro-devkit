#include "codec.h"
/* Mask for the bit EN of the I2S CFGR register */
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

const uint16_t w8731_init_data[] =
{
    0x017,			// Reg 00: Left Line In (0dB, mute off)
    0x017,			// Reg 01: Right Line In (0dB, mute off)
    0x079,			// Reg 02: Left Headphone out (0dB)
    0x079,			// Reg 03: Right Headphone out (0dB)
    0x012,			// Reg 04: Analog Audio Path Control (DAC sel, Mute Mic)
    0x000,			// Reg 05: Digital Audio Path Control
    0x062,			// Reg 06: Power Down Control (Clkout, Osc, Mic Off)
//	0x00E,			// Reg 07: Digital Audio Interface Format (i2s, 32-bit, slave)
    0x002,			// Reg 07: Digital Audio Interface Format (i2s, 16-bit, slave)
    0x000,			// Reg 08: Sampling Control (Normal, 256x, 48k ADC/DAC)
    0x001			// Reg 09: Active Control
};

/* The 7 bits Codec address (sent through I2C interface) */
#define CODEC_ADDRESS           (W8731_ADDR_0<<1)

/* local vars */
__IO uint32_t  CODECTimeout = CODEC_LONG_TIMEOUT;
__IO uint8_t OutputDev = 0;


/**
  * @brief  Inserts a delay time (not accurate timing).
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
static void Delay(__IO uint32_t nCount)
{
    for (; nCount != 0; nCount--);
}
/**
  * @brief  Resets the audio codec. It restores the default configuration of the
  *         codec (this function shall be called before initializing the codec).
  * @note   This function calls an external driver function: The IO Expander driver.
  * @param  None
  * @retval None
  */
void Codec_Reset(void)
{
    uint8_t i;

    Codec_WriteRegister(0x0f, 0);

    /* Load default values */
    for (i = 0;i < W8731_NUM_REGS;i++)
    {
        Codec_WriteRegister(i, w8731_init_data[i]);
    }

}

/**
  * @brief  Writes a Byte to a given register into the audio codec through the
            control interface (I2C)
  * @param  RegisterAddr: The address (location) of the register to be written.
  * @param  RegisterValue: the Byte value to be written into destination register.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t Codec_WriteRegister(uint8_t RegisterAddr, uint16_t RegisterValue)
{
    uint32_t result = 0;

    /* Assemble 2-byte data in WM8731 format */
    uint8_t Byte1 = ((RegisterAddr << 1) & 0xFE) | ((RegisterValue >> 8) & 0x01);
    uint8_t Byte2 = RegisterValue & 0xFF;

    /*!< While the bus is busy */
    CODECTimeout = CODEC_LONG_TIMEOUT;
    while (I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY))
    {
        if ((CODECTimeout--) == 0)
            return Codec_TIMEOUT_UserCallback();
    }

    /* Start the config sequence */
    I2C_GenerateSTART(I2C2, ENABLE);

    // /* Test on EV5 and clear it */
    CODECTimeout = CODEC_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if ((CODECTimeout--) == 0)
            return Codec_TIMEOUT_UserCallback();
    }

    /* Transmit the slave address and enable writing operation */
    I2C_Send7bitAddress(I2C2, CODEC_ADDRESS, I2C_DIRECTION_TRANSMIT);

    /* Test on EV6 and clear it */
    CODECTimeout = CODEC_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if ((CODECTimeout--) == 0)
            return Codec_TIMEOUT_UserCallback();
    }

    /* Transmit the first address for write operation */
    I2C_SendData(I2C2, Byte1);

    /* Test on EV8 and clear it */
    CODECTimeout = CODEC_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
    {
        if ((CODECTimeout--) == 0)
            return Codec_TIMEOUT_UserCallback();
    }

    /* Prepare the register value to be sent */
    I2C_SendData(I2C2, Byte2);

    /*!< Wait till all data have been physically transferred on the bus */
    CODECTimeout = CODEC_LONG_TIMEOUT;
    while (!I2C_GetFlagStatus(I2C2, I2C_FLAG_BTF))
    {
        if ((CODECTimeout--) == 0)
            return Codec_TIMEOUT_UserCallback();
    }

    /* End the configuration sequence */
    I2C_GenerateSTOP(I2C2, ENABLE);

    /* Return the verifying value: 0 (Passed) or 1 (Failed) */
    return result;
}
