#ifndef MYUSART_H
#define MYUSART_H
#include "stm32f10x.h"
#include "MyTimer.h"
#include "MyGPIO.h"

typedef struct {
	USART_TypeDef * Uart ; // TIM1 à TIM4
	int Debit;
	int Prio;
} MyUART_Struct_TypeDef ;

//dans la datasheet pages 29-30-31
#define TxPin_UART1 9
#define RxPin_UART1 10
#define GPIO_UART1 GPIOA

#define TxPin_UART2 2
#define RxPin_UART2 3
#define GPIO_UART2 GPIOA

#define TxPin_UART3 10
#define RxPin_UART3 11
#define GPIO_UART3 GPIOB

//Initialisation de l'USART
void MyUSART_Init( MyUART_Struct_TypeDef * MyUart);

//Réception des octets 
#define MyUSART_Reception() ( USART1 -> CR1 |= USART_CR1_RE )

//Envoyer(transmettre) des octets
#define MyUSART_Transmission() ( USART1 -> CR1 |= USART_CR1_TE )

//Attendre pour recevoir les bits 
#define MyUSART_WaitOnSend(muart) ((muart.Uart->SR & USART_SR_TXE) == 0)
char MyUSART_Get_Byte(USART_TypeDef * Uart);

//Envoyer des octets
void MyUSART_Send_Byte(USART_TypeDef * Uart, char * Byte);

//envoyer des messages
void MyUSART_Send_Mess(USART_TypeDef * Uart, char Byte);


//Activer Interruption
void MyUSART_Active_IT (MyUART_Struct_TypeDef * MyUart, void(*IT_function) (void));
#endif