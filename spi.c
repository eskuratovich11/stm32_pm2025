#include "spi.h"
#include <stm32f10x.h>

void SPI1_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN | RCC_APB2ENR_IOPAEN;
    
    GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5);
    GPIOA->CRL |= GPIO_CRL_CNF5_1 | GPIO_CRL_MODE5;
    
    GPIOA->CRL &= ~(GPIO_CRL_CNF6 | GPIO_CRL_MODE6);
    GPIOA->CRL |= GPIO_CRL_CNF6_0;
    
    GPIOA->CRL &= ~(GPIO_CRL_CNF7 | GPIO_CRL_MODE7);
    GPIOA->CRL |= GPIO_CRL_CNF7_1 | GPIO_CRL_MODE7;
    
    SPI1->CR1 = 0;
    SPI1->CR1 |= SPI_CR1_MSTR;
    SPI1->CR1 |= SPI_CR1_SSM;
    SPI1->CR1 |= SPI_CR1_SSI;
    SPI1->CR1 |= SPI_CR1_BR_1;
    SPI1->CR1 |= SPI_CR1_SPE;
}

void SPI1_Write(uint8_t data) {
    while (!(SPI1->SR & SPI_SR_TXE));
    
    SPI1->DR = data;
    
    while (SPI1->SR & SPI_SR_BSY);
}

uint8_t SPI1_Read(void) {
    while (!(SPI1->SR & SPI_SR_TXE));
    SPI1->DR = 0xFF;
    
    while (!(SPI1->SR & SPI_SR_RXNE));
    
    return (uint8_t)SPI1->DR;
}