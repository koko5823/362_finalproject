#include "stm32f0xx.h"

void set_char_msg(int, char);
void nano_wait(unsigned int);
void game(void);
void internal_clock();
void check_wiring();
void autotest();
void GPIO_Init(void);
void Timer2_init(void);
void delay(int ms);
void servo_write(uint8_t angle);
void rotate(void);
int pos;


//===========================================================================
//servo
void Timer2_init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;


    GPIOA->MODER &= ~(0xFF);
    GPIOA->MODER |= 0xAA;
    GPIOA->AFR[0] &= ~(0xFFFF);
    GPIOA->AFR[0] |= 0x2222;


    TIM2->PSC = 479;
    TIM2->ARR = 1999;
    TIM2->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;
    TIM2->CCMR1 &= ~TIM_CCMR1_OC1M_0;
    TIM2->CCER |= TIM_CCER_CC1E;
    TIM2->CR1 |= TIM_CR1_CEN;
}


void rotate(void)
{
    TIM2->CCR1 = 150;// rotate0
    //15 is 0 degree
    //0 is 90 degree
    nano_wait(2000000000); // wait for 1 second
    //TIM2->CCR1 = 200; // set the servo to 180 degrees
}
void rotate1(void)//rotate to 90 degrees
{
    TIM2->CCR1 = 250; // set the servo to 180 degrees
}



int main(void) {
    internal_clock();
    Timer2_init();
    rotate();
    rotate1();

}
