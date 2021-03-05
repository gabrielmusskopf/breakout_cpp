#include "stm32f4xx.h"
#include <stdio.h>

int dado=0, dado2 = 0;

void manda1(int a)
{
	while(!((USART2->SR)&0x80)){};
	USART2->DR=a;
}

void manda2(int a)
{
	while(!((USART6->SR)&0x80)){};

		USART6->DR=a;
}

void USART6_IRQHandler(void)
{

	if(USART6->SR&0x20)
	{
		dado = USART6->DR;

	}
	USART6->SR&=~(0x20);

}
void USART2_IRQHandler(void)
{

	if(USART2->SR&0x20)
	{
		dado2 = USART2->DR;

	}
	USART2->SR&=~(0x20);
}


int main(void)
{
	RCC-> AHB1ENR=0x00000087;
	RCC-> APB2ENR=0x60000;
	RCC-> APB1ENR|=0x20000;
	RCC->APB2ENR|=0x00000020;
	GPIOA-> MODER|=0xA0; //PA2 e PA3 para função alternativa.
	GPIOA->AFR[0]=0x07700;
	USART2-> CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
	USART2-> CR1|=0x202C;
	USART2-> CR2=0x00;
	USART2-> CR3=0x00;
	USART2-> BRR|=(26<<4); //38400

	NVIC_EnableIRQ(USART2_IRQn);
	NVIC_SetPriority(USART2_IRQn, 1);
	USART6-> CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
	USART6-> CR1|=0x202C;
	USART6-> CR2=0x00;
	USART6-> CR3=0x00;
	USART6-> BRR|=(26<<4); //38400 = baud rate
	GPIOC->MODER|=0xA000; //PC6 e PC7 função alternativa rx no 6 e tx no 7
	GPIOC->AFR[0]=0x88000000;
	NVIC_EnableIRQ(USART6_IRQn);
	NVIC_SetPriority(USART6_IRQn, 2);

  while (1)
  {
	  if(dado2!=0)
	  {
		  manda2(dado2);
		  dado2=0;
	  }
	  if(dado!=0)
	  {
		  manda1(dado);
		  dado = 0;
	  }
  }
}

