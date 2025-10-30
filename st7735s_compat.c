#include "st7735s_compat.h"
#include "spi.h"
#include "tim.h"
#include "main.h"
#include "gpio.h"
#include "cmsis_os.h"

uint32_t tim_period = 999;
uint32_t tim_pulse;

void SPI_Init(void) {
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, RESET);
}

void Pin_CS_Low(void) {
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, RESET);
}

void Pin_CS_High(void) {
	HAL_GPIO_WritePin(TFT_CS_GPIO_Port, TFT_CS_Pin, SET);
}

void Pin_RES_High(void) {
	HAL_GPIO_WritePin(TFT_RST_GPIO_Port, TFT_RST_Pin, SET);
}

void Pin_RES_Low(void) {
	HAL_GPIO_WritePin(TFT_RST_GPIO_Port, TFT_RST_Pin, RESET);
}

void Pin_DC_High(void) {
	HAL_GPIO_WritePin(TFT_RS_GPIO_Port, TFT_RS_Pin, SET);
}

void Pin_DC_Low(void) {
	HAL_GPIO_WritePin(TFT_RS_GPIO_Port, TFT_RS_Pin, RESET);
}

extern uint8_t backlight_pct;
void Pin_BLK_Pct(uint8_t pct) {
    backlight_pct = pct;
    tim_pulse = pct*tim_period/100;
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, tim_pulse);
}

void SPI_send(uint16_t len, uint8_t *data) {
	Pin_CS_Low();
#if 0
	while (len--)
		HAL_SPI_Transmit(&hspi1, data++, 1, 0xF000);
#else
	HAL_SPI_Transmit(&hspi1, data, len, 0xF000);
	Pin_CS_High();
#endif
}

void SPI_TransmitCmd(uint16_t len, uint8_t *data) {
    Pin_DC_Low();
    SPI_send(len, data);
}

void SPI_TransmitData(uint16_t len, uint8_t *data) {
    Pin_DC_High();
    SPI_send(len, data);
}

void SPI_Transmit(uint16_t len, uint8_t *data) {
    SPI_TransmitCmd(1, data++);
    if (--len)
       SPI_TransmitData(len, data);
}

void _Delay(uint32_t d) {
    osDelay(d);
}

