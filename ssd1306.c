#include "ssd1306.h"
#include "spi.h"
#include <stm32f10x.h>

static uint8_t ssd1306_buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];

#define CS_PIN   GPIO_Pin_2
#define RST_PIN  GPIO_Pin_3
#define DC_PIN   GPIO_Pin_4

#define CS_LOW()   GPIOA->BSRR = GPIO_BSRR_BR2
#define CS_HIGH()  GPIOA->BSRR = GPIO_BSRR_BS2
#define RST_LOW()  GPIOA->BSRR = GPIO_BSRR_BR3
#define RST_HIGH() GPIOA->BSRR = GPIO_BSRR_BS3
#define DC_LOW()   GPIOA->BSRR = GPIO_BSRR_BR4
#define DC_HIGH()  GPIOA->BSRR = GPIO_BSRR_BS4

static void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms * 8000; i++) {
        __NOP();
    }
}

void SSD1306_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    
    GPIOA->CRL &= ~(GPIO_CRL_CNF2 | GPIO_CRL_MODE2);
    GPIOA->CRL |= GPIO_CRL_MODE2;
    
    GPIOA->CRL &= ~(GPIO_CRL_CNF3 | GPIO_CRL_MODE3);
    GPIOA->CRL |= GPIO_CRL_MODE3;
    
    GPIOA->CRL &= ~(GPIO_CRL_CNF4 | GPIO_CRL_MODE4);
    GPIOA->CRL |= GPIO_CRL_MODE4;
    
    CS_HIGH();
    RST_HIGH();
    DC_LOW();
    
    SPI1_Init();
    
    RST_HIGH();
    delay_ms(10);
    RST_LOW();
    delay_ms(10);
    RST_HIGH();
    delay_ms(10);
    
    SSD1306_Command(SSD1306_DISPLAYOFF);
    
    SSD1306_Command(SSD1306_SETDISPLAYCLOCKDIV);
    SSD1306_Command(0x80);
    
    SSD1306_Command(SSD1306_SETMULTIPLEX);
    SSD1306_Command(SSD1306_HEIGHT - 1);
    
    SSD1306_Command(SSD1306_SETDISPLAYOFFSET);
    SSD1306_Command(0x0);
    
    SSD1306_Command(SSD1306_SETSTARTLINE | 0x0);
    
    SSD1306_Command(SSD1306_CHARGEPUMP);
    SSD1306_Command(0x14);
    
    SSD1306_Command(SSD1306_MEMORYMODE);
    SSD1306_Command(0x00);
    
    SSD1306_Command(SSD1306_SEGREMAP | 0x1);
    SSD1306_Command(SSD1306_COMSCANDEC);
    
    SSD1306_Command(SSD1306_SETCOMPINS);
    SSD1306_Command(0x12);
    
    SSD1306_Command(SSD1306_SETCONTRAST);
    SSD1306_Command(0xCF);
    
    SSD1306_Command(SSD1306_SETPRECHARGE);
    SSD1306_Command(0xF1);
    
    SSD1306_Command(SSD1306_SETVCOMDETECT);
    SSD1306_Command(0x40);
    
    SSD1306_Command(SSD1306_DISPLAYALLON_RESUME);
    SSD1306_Command(SSD1306_NORMALDISPLAY);
    
    SSD1306_Command(SSD1306_DISPLAYON);
    
    SSD1306_Clear();
    SSD1306_Display();
}

void SSD1306_Command(uint8_t cmd) {
    CS_LOW();
    DC_LOW();
    SPI1_Write(cmd);
    CS_HIGH();
}

void SSD1306_Data(uint8_t data) {
    CS_LOW();
    DC_HIGH();
    SPI1_Write(data);
    CS_HIGH();
}

void SSD1306_Clear(void) {
    for (uint16_t i = 0; i < sizeof(ssd1306_buffer); i++) {
        ssd1306_buffer[i] = 0x00;
    }
}

void SSD1306_Display(void) {
    SSD1306_Command(SSD1306_COLUMNADDR);
    SSD1306_Command(0);
    SSD1306_Command(SSD1306_WIDTH - 1);
    
    SSD1306_Command(SSD1306_PAGEADDR);
    SSD1306_Command(0);
    SSD1306_Command(7);
    
    CS_LOW();
    DC_HIGH();
    for (uint16_t i = 0; i < sizeof(ssd1306_buffer); i++) {
        SPI1_Write(ssd1306_buffer[i]);
    }
    CS_HIGH();
}

void SSD1306_SetPixel(uint8_t x, uint8_t y, uint8_t color) {
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) {
        return;
    }
    
    if (color) {
        ssd1306_buffer[x + (y / 8) * SSD1306_WIDTH] |= (1 << (y % 8));
    } else {
        ssd1306_buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }
}

void SSD1306_DrawChessboard(void) {
    uint8_t cell_size = 8;
    
    SSD1306_Clear();
    
    for (uint8_t row = 0; row < SSD1306_HEIGHT / cell_size; row++) {
        for (uint8_t col = 0; col < SSD1306_WIDTH / cell_size; col++) {
            uint8_t color = (row + col) % 2;
            
            if (color) {
                for (uint8_t y = 0; y < cell_size; y++) {
                    for (uint8_t x = 0; x < cell_size; x++) {
                        SSD1306_SetPixel(col * cell_size + x, row * cell_size + y, 1);
                    }
                }
            }
        }
    }
    
    SSD1306_Display();
}