#ifndef ADS1220_H
#define ADS1220_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx_hal.h"   /* ваша серия */

/* ── Команды ── */
#define ADS1220_CMD_RESET       0x06
#define ADS1220_CMD_START       0x08
#define ADS1220_CMD_POWERDOWN   0x02
#define ADS1220_CMD_RDATA       0x10
#define ADS1220_CMD_RREG(a,n)   (0x20 | (((a)&0x03)<<2) | ((n)&0x03))
#define ADS1220_CMD_WREG(a,n)   (0x40 | (((a)&0x03)<<2) | ((n)&0x03))

/* ── MUX ── */
#define ADS1220_MUX_AIN0_AIN1   0x00
#define ADS1220_MUX_AIN0_AIN2   0x10
#define ADS1220_MUX_AIN0_AIN3   0x20
#define ADS1220_MUX_AIN1_AIN2   0x30
#define ADS1220_MUX_AIN1_AIN3   0x40
#define ADS1220_MUX_AIN2_AIN3   0x50
#define ADS1220_MUX_AIN1_AIN0   0x60
#define ADS1220_MUX_AIN3_AIN2   0x70
#define ADS1220_MUX_AIN0_AVSS   0x80
#define ADS1220_MUX_AIN1_AVSS   0x90
#define ADS1220_MUX_AIN2_AVSS   0xA0
#define ADS1220_MUX_AIN3_AVSS   0xB0
#define ADS1220_MUX_VREF_DIV4   0xC0
#define ADS1220_MUX_AVDD_DIV4   0xF0

/* ── GAIN ── */
#define ADS1220_GAIN_1          0x00
#define ADS1220_GAIN_2          0x02
#define ADS1220_GAIN_4          0x04
#define ADS1220_GAIN_8          0x06
#define ADS1220_GAIN_16         0x08
#define ADS1220_GAIN_32         0x0A
#define ADS1220_GAIN_64         0x0C
#define ADS1220_GAIN_128        0x0E
#define ADS1220_PGA_BYPASS      0x01

/* ── DR / MODE ── */
#define ADS1220_DR_20           0x00
#define ADS1220_DR_45           0x20
#define ADS1220_DR_90           0x40
#define ADS1220_DR_175          0x60
#define ADS1220_DR_330          0x80
#define ADS1220_DR_600          0xA0
#define ADS1220_DR_1000         0xC0

#define ADS1220_MODE_NORMAL     0x00
#define ADS1220_MODE_DUTY       0x08
#define ADS1220_MODE_TURBO      0x10

#define ADS1220_CM_SINGLE       0x00
#define ADS1220_CM_CONTINUOUS   0x04
#define ADS1220_TS_ON           0x02
#define ADS1220_BCS_ON          0x01

/* ── VREF / FIR / IDAC ── */
#define ADS1220_VREF_INT        0x00
#define ADS1220_VREF_EXT0       0x40
#define ADS1220_VREF_EXT1       0x80
#define ADS1220_VREF_AVDD       0xC0

#define ADS1220_FIR_NONE        0x00
#define ADS1220_FIR_50_60       0x10
#define ADS1220_FIR_50          0x20
#define ADS1220_FIR_60          0x30

#define ADS1220_IDAC_OFF        0x00
#define ADS1220_IDAC_10uA       0x01
#define ADS1220_IDAC_50uA       0x02
#define ADS1220_IDAC_100uA      0x03
#define ADS1220_IDAC_250uA      0x04
#define ADS1220_IDAC_500uA      0x05
#define ADS1220_IDAC_1000uA     0x06
#define ADS1220_IDAC_1500uA     0x07

/* ── API ── */
void     ADS1220_Init(void);
void     ADS1220_Reset(void);
void     ADS1220_WriteReg(uint8_t reg, uint8_t value);
uint8_t  ADS1220_ReadReg(uint8_t reg);
void     ADS1220_WriteRegs(uint8_t start, const uint8_t *buf, uint8_t n);
int32_t  ADS1220_ReadData(void);
void     ADS1220_Start(void);
void     ADS1220_PowerDown(void);
int32_t  ADS1220_ReadSingleShot(void);
float    ADS1220_CodeToVolt(int32_t code, uint8_t gain, float vref);
#ifdef __cplusplus
}
#endif

#endif

