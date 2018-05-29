 #include "uart.h"
 
 
 void USART1_Send(uint8_t byte) 
{
  while(!(USART1->SR & USART_SR_TC));
  USART1->DR = byte;
}

void USART1_Send_String(uint8_t* out, uint8_t size) 
{
	for (int i = 0; i < size; i++)
    USART1_Send(out[i++]);
}

void USART1_IRQHandler()
{
	uint8_t command = USART1->DR;
	GPIOD->ODR = 0;
	switch (command)
  {
		case Forward:
			GPIOD->ODR = GPIO_ODR_OD13;
			break;
		case Back:
			GPIOD->ODR = GPIO_ODR_OD15;
			break;
    case Right:
      GPIOD->ODR = GPIO_ODR_OD14;
      break;
    case Left:
      GPIOD->ODR = GPIO_ODR_OD12;
      break;
    case Up:
      GPIOD->ODR = GPIO_ODR_OD13|GPIO_ODR_OD15;
      break;
    case Down:
      GPIOD->ODR = GPIO_ODR_OD12|GPIO_ODR_OD14;
      break;
    case CW:
			GPIOD->ODR = GPIO_ODR_OD12|GPIO_ODR_OD13;
      break;
    case CCW:
      GPIOD->ODR = GPIO_ODR_OD14|GPIO_ODR_OD15;
      break;
		case Error:
			GPIOD->ODR = GPIO_ODR_OD12|GPIO_ODR_OD13|GPIO_ODR_OD14|GPIO_ODR_OD15;
			break;
    default:
      GPIOD->ODR = 0;
      break;
	}
}
 
void USART1_Init() 
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; //На PA9 и PA10 будет UART
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	GPIOA->MODER |= GPIO_MODER_MODER9_1|GPIO_MODER_MODER10_1|GPIO_MODER_MODER8_0; //Alternate function
	GPIOA->AFR[1] |= 0x00000770; // AF7 = UART
	USART1->BRR = 0x168;
	USART1->CR1 |= USART_CR1_RE|USART_CR1_TE|USART_CR1_RXNEIE|USART_CR1_UE;
	__enable_irq();
	NVIC_EnableIRQ(USART1_IRQn);
	GPIOA->ODR |= GPIO_ODR_ODR_8;
}