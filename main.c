#include <stdint.h>
#include <stm32f10x.h>
#include "spi.h"
#include "ssd1306.h"

void delay(uint32_t cycles) {
  for (uint32_t i = 0; i < cycles; i++) {
    __NOP();
  }
}

int main(void) {
  SPI1_Init();
  
  SSD1306_Init();
  
  SSD1306_DrawChessboard();
  
  while (1) {
    delay(1000000);
  }
}