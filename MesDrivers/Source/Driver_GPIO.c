#include "stm32f10x.h"
#include "MyGPIO.h"
//fonction qui active les GPIO


void MyGPIO_Init(MyGPIO_Struct_TypeDef * GPIOStructPtr)
{

		if (GPIOStructPtr->GPIO == GPIOA) RCC->APB2ENR |=(0x01 << 2);
		else if (GPIOStructPtr->GPIO ==  GPIOB) RCC->APB2ENR |=(0x01 << 3);
		else RCC->APB2ENR |=(0x01 << 4);
		if (GPIOStructPtr->GPIO_Pin< '8')   
		{
			GPIOStructPtr->GPIO->CRL &= ~(0xF<<(int)GPIOStructPtr->GPIO_Pin*4);
			GPIOStructPtr->GPIO->CRL |= (GPIOStructPtr->GPIO_Conf<<(int)GPIOStructPtr->GPIO_Pin*4);
		}	
		else{ 
			GPIOStructPtr->GPIO->CRH &= ~(0xF<<(int)(GPIOStructPtr->GPIO_Pin-8)*4);
			GPIOStructPtr->GPIO->CRH |= (GPIOStructPtr->GPIO_Conf <<(int)(GPIOStructPtr->GPIO_Pin-8)*4);
		
		}
		//dans le cas où on peut faire un pull-up
		if (GPIOStructPtr->GPIO_Conf==In_PullUp) GPIOStructPtr->GPIO->ODR |=(0x01<<(int)GPIOStructPtr->GPIO_Pin);
}

int MyGPIO_Read ( GPIO_TypeDef * GPIO , char GPIO_Pin )
{
	return (GPIO->ODR&=(0x01 << (int)GPIO_Pin));
}

void MyGPIO_Set ( GPIO_TypeDef * GPIO , char GPIO_Pin ){
	
	GPIO->BSRR |= (0x01 << (int)GPIO_Pin);
}

void MyGPIO_Reset ( GPIO_TypeDef * GPIO , char GPIO_Pin )
{
	
	GPIO->BRR |= (0x01 << (int)GPIO_Pin);
}


//fonction pour mettre on ou off la led
void MyGPIO_Toggle ( GPIO_TypeDef * GPIO , char GPIO_Pin )
{
	if(MyGPIO_Read(GPIO,GPIO_Pin)) MyGPIO_Reset(GPIO,GPIO_Pin);
	else{ MyGPIO_Set(GPIO,GPIO_Pin);
	}
}

