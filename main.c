#include <stdint.h>
#include <stm32f10x.h>

void delay(uint32_t cycles) {
  for (uint32_t i = 0; i < cycles; i++) {
    __NOP();
  }
}

uint32_t blink_period = 8000000;
int8_t speed_stage = 0;

void check_buttons(void) {
  if ((GPIOC->IDR & GPIO_IDR_IDR14) == 0) {
    delay(10000);
    if ((GPIOC->IDR & GPIO_IDR_IDR14) == 0) {
      if (speed_stage < 6) {
        speed_stage++;
        blink_period /= 2;
      }
      while ((GPIOC->IDR & GPIO_IDR_IDR14) == 0) {
      }
    }
  }

  if ((GPIOC->IDR & GPIO_IDR_IDR15) == 0) {
    delay(10000);
    if ((GPIOC->IDR & GPIO_IDR_IDR15) == 0) {
      if (speed_stage > -6) {
        speed_stage--;
        blink_period *= 2;
      }
      while ((GPIOC->IDR & GPIO_IDR_IDR15) == 0) {
      }
    }
  }
}

int main(void) {
  RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

  GPIOC->CRH &= ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13);
  GPIOC->CRH |= GPIO_CRH_MODE13_0;

  GPIOC->CRH &=
      ~(GPIO_CRH_CNF14 | GPIO_CRH_MODE14 | GPIO_CRH_CNF15 | GPIO_CRH_MODE15);
  GPIOC->CRH |= (GPIO_CRH_CNF14_1 | GPIO_CRH_CNF15_1);
  GPIOC->ODR |= (GPIO_ODR_ODR14 | GPIO_ODR_ODR15);

  while (1) {
    check_buttons();

    GPIOC->ODR ^= GPIO_ODR_ODR13;

    delay(blink_period);
  }
}
