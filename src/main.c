/*
**
**                           Main.c
**
**
**********************************************************************/
/*
   Last committed:     $Revision: 00 $
   Last changed by:    $Author: $
   Last changed date:  $Date:  $
   ID:                 $Id:  $

**********************************************************************/
#include "stm32f0xx_conf.h"

#define SCK_PIN  GPIO_Pin_5
#define MISO_PIN GPIO_Pin_6
#define MOSI_PIN GPIO_Pin_7
#define NSS_PIN  GPIO_Pin_4

void spi_init(void)
{

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_0);

    GPIO_InitTypeDef GPIO_Init_Structure;
    GPIO_Init_Structure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init_Structure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init_Structure.GPIO_Pin = SCK_PIN|MISO_PIN|MOSI_PIN;
    GPIO_Init_Structure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init_Structure.GPIO_Speed = GPIO_Speed_Level_3;
    GPIO_Init(GPIOA, &GPIO_Init_Structure);

    GPIO_Init_Structure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init_Structure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init_Structure.GPIO_Pin = NSS_PIN;
    GPIO_Init_Structure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init_Structure.GPIO_Speed = GPIO_Speed_Level_3;

    GPIO_Init(GPIOA, &GPIO_Init_Structure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    SPI_InitTypeDef SPI_Init_Structure;
    SPI_Init_Structure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
    SPI_Init_Structure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_Init_Structure.SPI_CPOL = SPI_CPOL_Low;
    SPI_Init_Structure.SPI_DataSize = SPI_DataSize_8b;
    SPI_Init_Structure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_Init_Structure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init_Structure.SPI_NSS = SPI_NSS_Soft;
    SPI_Init_Structure.SPI_Mode = SPI_Mode_Master;
    SPI_Init(SPI1, &SPI_Init_Structure);
    SPI_Cmd(SPI1, ENABLE);


}


void spi_send_byte(uint16_t byte)
{
    GPIO_ResetBits(GPIOA, NSS_PIN);
     *((__IO uint16_t*)&SPI1->DR) = byte;
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
    GPIO_SetBits(GPIOA, NSS_PIN);
}

int main(void)
{
    spi_init();
    spi_send_byte(0x2A);
    while(1)
    {

    }
}
