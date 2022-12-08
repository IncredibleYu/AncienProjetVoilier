#include "MyUSART.h"

static void (*HandlerUart1)(void);
static void (*HandlerUart2)(void);
static void (*HandlerUart3)(void);

//Initialisation de l'USART
//Activation clock
//Doc RM0008 pages 156, 832
void MyUSART_Init (MyUART_Struct_TypeDef * MyUart) {
	if (MyUart->Uart == USART1){
		RCC -> APB2ENR |= RCC_APB2ENR_USART1EN;
		MyUart->Uart->BRR = 72000000/(MyUart->Debit);} //Baude rate 72MHz
	
	if (MyUart->Uart == USART2){
		RCC -> APB1ENR |= RCC_APB1ENR_USART2EN;
		MyUart->Uart->BRR = 36000000/(MyUart->Debit);} //Baude rate 36MHz
	
	if (MyUart->Uart == USART3){
		RCC -> APB1ENR |= RCC_APB1ENR_USART3EN;
		MyUart->Uart->BRR = 36000000/(MyUart->Debit);} //Baude rate 36MHz
	
	//USART Actication
	MyUart->Uart ->CR1 |= USART_CR1_UE;
	//On a 1 bit de Start, 8 bits de données (taille de la donnée)
	MyUart->Uart->CR1 &= ~USART_CR1_M; 
	//On choisit de mettre STOP à 00, pour un 1 bit de stop
	MyUart->Uart->CR2 &= ~USART_CR2_STOP; 
	
		//Réception des octets( Receiver enable)
	MyUart->Uart -> CR1 |= USART_CR1_RE ;
		//Tansmettre des octets (Transmitter enable)
	MyUart->Uart -> CR1 |= USART_CR1_TE ;
		
		//On active la transmission en mettant el bit TE de USART_CR1 a 1
	//USART->CR1 |= (1 << 3);
		
	//Ecriture du caractere a envoyer dans USART_DR
	//USART->DR = caractere;
}

//Activation interupteur
void MyUSART_Active_IT (MyUART_Struct_TypeDef * MyUart, void(*IT_function) (void)) {
	//Envoie d'une demande d'interruption validée
	MyUart->Uart->CR1 |= USART_CR1_RXNEIE;
	
	if (MyUart->Uart == USART1){
		HandlerUart1 = IT_function;
		NVIC->IP[USART1_IRQn] = MyUart->Prio <<4;
		NVIC->ISER[1] |= NVIC_ISER_SETENA_5;}
	
	if (MyUart->Uart == USART2){
		HandlerUart2 = IT_function;
		NVIC->IP[USART2_IRQn] = MyUart->Prio <<4;
		NVIC->ISER[1] |= NVIC_ISER_SETENA_6;}
	
	if (MyUart->Uart == USART3){
		HandlerUart3 = IT_function;
		NVIC->IP[USART3_IRQn] = MyUart->Prio <<4;
		NVIC->ISER[1] |= NVIC_ISER_SETENA_7;}
}

//Recevoir des octets (data du voilier)
char MyUSART_Get_Byte(USART_TypeDef * Uart){
	return (signed char) Uart->DR;}

void USART1_IRQHandler(void){
	 // On remet le flag à 0
	USART1->SR &= ~USART_SR_RXNE;
	//Message = USART1->DR;
	if (HandlerUart1 !=0){
	(*HandlerUart1) ();}
	}

void USART2_IRQHandler(void){
	// On remet le flag à 0
	USART2->SR &= ~USART_SR_RXNE; 
	//Message = USART2->DR;
	if (HandlerUart2 !=0){
	(*HandlerUart2) ();}
	}

void USART3_IRQHandler(void){
	 // On remet le flag à 0
	USART3->SR &= ~USART_SR_RXNE;
	//Message = USART3->DR;
	if (HandlerUart3 !=0){
	(*HandlerUart3) ();}
}

//Envoyer des octes (Envoyer du data au voilier (vitesse, etc..))
	void MyUSART_Send_Byte(USART_TypeDef * Uart, char *Byte){
		while (*Byte != '\0') {
		while(!(Uart->SR & USART_SR_TXE));
		// Ecriture de la donnée dans le registre DR
		Uart->DR = *Byte; 
		Byte++;
	}
}
	
void MyUSART_Send_Mess(USART_TypeDef * Uart, char Byte){
			Uart->DR = Byte; 
}
