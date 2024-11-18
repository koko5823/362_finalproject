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
void init_spi1();
void spi_cmd(unsigned int data);
void spi_data(unsigned int data);
void spi1_init_oled();
void spi1_display1(const char *string);
void spi1_display2(const char *string);

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

void init_spi1() {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER &= ~0xc000fc00;
    GPIOA->MODER |= 0x8000a800;
    GPIOA->AFR[1] &= ~0xf0000000;
    GPIOA->AFR[0] &= ~0xfff00000;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    SPI1->CR1 &= ~SPI_CR1_SPE;
    SPI1->CR1 |= SPI_CR1_MSTR | SPI_CR1_BR;
    SPI1->CR2 = 0x900;
    SPI1->CR2 |=  SPI_CR2_SSOE | SPI_CR2_NSSP;
    SPI1->CR1 |= SPI_CR1_SPE;
}

void spi_cmd(unsigned int data) {
    while(!(SPI1->SR & SPI_SR_TXE)) {}
    SPI1->DR = data;
}
void spi_data(unsigned int data) {
    spi_cmd(data | 0x200);
}
void spi1_init_oled() {
    nano_wait(1000000);
    spi_cmd(0x38);
    spi_cmd(0x08);
    spi_cmd(0x01);
    nano_wait(2000000);
    spi_cmd(0x06);
    spi_cmd(0x02);
    spi_cmd(0x0c);
}
void spi1_display1(const char *string) {
    spi_cmd(0x02);
    while(*string != '\0') {
        spi_data(*string);
        string++;
    }
}
void spi1_display2(const char *string) {
    spi_cmd(0xc0);
    while(*string != '\0') {
        spi_data(*string);
        string++;
    }
}

int main(void) {
    internal_clock();
    Timer2_init();
    rotate();
    rotate1();

}
