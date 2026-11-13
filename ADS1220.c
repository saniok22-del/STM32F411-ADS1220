#include "ads1220.h"
#include "main.h"

extern SPI_HandleTypeDef hspi1;
void  CS_LOW(void)
{	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);}
void CS_HIGH(void){
  HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);}

/* ──────────────────────────────────────────── */
void ADS1220_Init(void)
{
    
    CS_HIGH();
    HAL_Delay(1);
    ADS1220_Reset();
}

/* ──────────────────────────────────────────── */
void ADS1220_Reset(void)
{
    uint8_t cmd = ADS1220_CMD_RESET;
    CS_LOW();
    HAL_SPI_Transmit(&hspi1, &cmd, 1, 10);
    CS_HIGH();
    HAL_Delay(1);   /* t_RESET ~ 50 мкс; 1 мс с запасом */
}

/* ──────────────────────────────────────────── */
uint8_t ADS1220_ReadReg(uint8_t reg)
{
    uint8_t cmd = ADS1220_CMD_RREG(reg, 0);   /* 1 регистр */
    uint8_t val = 0;
    CS_LOW();
    HAL_SPI_Transmit(&hspi1, &cmd, 1, 10);
		HAL_StatusTypeDef st = HAL_SPI_Receive (&hspi1, &val, 1, 10);
    CS_HIGH();
    return val;
}

/* ──────────────────────────────────────────── */
void ADS1220_WriteReg(uint8_t reg, uint8_t value)
{
    uint8_t buf[2] = { ADS1220_CMD_WREG(reg, 0), value };
    CS_LOW();
    HAL_SPI_Transmit(&hspi1, buf, 2, 10);
    CS_HIGH();
}

/* ──────────────────────────────────────────── */
/* Записать несколько регистров подряд начиная с reg */
void ADS1220_WriteRegs(uint8_t reg, const uint8_t *buf, uint8_t n)
{
    uint8_t cmd = ADS1220_CMD_WREG(reg, n - 1);
    CS_LOW();
    HAL_SPI_Transmit(&hspi1, &cmd, 1, 10);
    HAL_SPI_Transmit(&hspi1, (uint8_t *)buf, n, 10);
    CS_HIGH();
}

/* ──────────────────────────────────────────── */
void ADS1220_Start(void)
{
    uint8_t cmd = ADS1220_CMD_START;
    CS_LOW();
    HAL_SPI_Transmit(&hspi1, &cmd, 1, 10);
    CS_HIGH();
}

/* ──────────────────────────────────────────── */
void ADS1220_PowerDown(void)
{
    uint8_t cmd = ADS1220_CMD_POWERDOWN;
    CS_LOW();
    HAL_SPI_Transmit(&hspi1, &cmd, 1, 10);
    CS_HIGH();
}

/* ──────────────────────────────────────────── */
/* Чтение 24-бит, знаковое (дополнительный код) */
int32_t ADS1220_ReadData(void)
{
    uint8_t cmd = ADS1220_CMD_RDATA;
    uint8_t rx[3];

    CS_LOW();
    HAL_SPI_Transmit(&hspi1, &cmd, 1, 10);
     HAL_StatusTypeDef st = HAL_SPI_Receive (&hspi1, rx, 3, 10);
    CS_HIGH();
		if (st != HAL_OK) return HAL_ERROR;
    int32_t code = ((int32_t)rx[0] << 16)
                 | ((int32_t)rx[1] <<  8)
                 |  (int32_t)rx[2];

    /* Знаковое расширение 24→32 */
    if (code & 0x800000) code |= 0xFF000000;
    return code;
}

/* ──────────────────────────────────────────── */
/* Single-shot: запуск + ожидание DRDY + чтение */
int32_t ADS1220_ReadSingleShot(void)
{
    ADS1220_Start();

    /* Ждём падения DRDY (или опрос пина; здесь — простой polling) */
    uint32_t t0 = HAL_GetTick();
    while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_SET) {
        if (HAL_GetTick() - t0 > 100) return 0x7FFFFFFF;   /* таймаут */
    }
    return ADS1220_ReadData();
}

/* ──────────────────────────────────────────── */
float ADS1220_CodeToVolt(int32_t code, uint8_t gain, float vref)
{
    return ((float)code * vref) / (8388608.0f * (float)gain);
}
