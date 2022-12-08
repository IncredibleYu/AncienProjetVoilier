#include "CommunicDist.h"
#include "MyADC.h"
#include "MyGPIO.h"
#include "MyUSART.h"
#include <stdlib.h>

#define ADC ADC1
#define ADCchan 10
#define Duree_Ech 10000
#define ref 20

static MyUART_Struct_TypeDef muart;
static MyGPIO_Struct_TypeDef ServoMtr;
static MyGPIO_Struct_TypeDef bitsens;
static MyGPIO_Struct_TypeDef uartTx;
static MyGPIO_Struct_TypeDef uartRx;
static MyTimer_Struct_TypeDef timServo;

static const char bat_faible[15] = "Faible Batterie";
static const char msg_angle[7] = "Angle :";
static const char msg_alerte[19] = "Alerte Chavirement!";

//Envoyer un tableau des 3 premiers chiffres de l'argument en characters
char * toArray(int number) {
    int n = 3;
    int i;
    char *numberArray = calloc(n, sizeof(char));
    for (i = n-1; i >= 0; --i, number /= 10) {
        numberArray[i] = (number % 10) + '0'; }
    return numberArray; }

//Initialisation l'Adc pour le niveau de batterie

void CommunicDist_InitADC(void){
	Init_ADC_Single_Conv(ADC, Duree_Ech); }

//Envoyer le niveau de la batterie
int CommunicDist_NiveauBat(void){
	ADC_Start(ADC, ADCchan);
	return ADC->DR; }


//PE en decimal c'est 1145
//Envoyer 0 si Batterie "pleine" et 1 si batterie faible
int CommunicDist_FaibleBatterie(char PourBatFaible){
	if ( CommunicDist_NiveauBat()*100 <= PourBatFaible*1145){
		return 1;}
	else {
		return 0; }
}

//Initialisation de l'UART
void CommunicDist_InitUart (){
	// Configurer USART
  muart.Debit=9600;
	muart.Prio = 4;
	muart.Uart=USART1;
	
//Configurer Bit de Sens
	bitsens.GPIO = GPIOA;
	bitsens.GPIO_Conf = Out_Ppull;
	bitsens.GPIO_Pin = 8;
	MyGPIO_Init (&bitsens);
	
//Configurer ServoMtr
	ServoMtr.GPIO = GPIOB;
	ServoMtr.GPIO_Conf = AltOut_Ppull;
	ServoMtr.GPIO_Pin = 6;
	MyGPIO_Init (&ServoMtr);
	
//Configurer Timer
	timServo.Timer = TIM4;
	timServo.ARR = 720-1;
	timServo.PSC = 1-1;
	MyTimer_Base_Init (&timServo);
	
//Configurer Uart Tx
	uartTx.GPIO = GPIO_UART1;
	uartTx.GPIO_Conf = AltOut_Ppull;
	uartTx.GPIO_Pin = TxPin_UART1;
	MyGPIO_Init ( &uartTx );

//Configurer Uart Rx
	uartRx.GPIO = GPIO_UART1;
	uartRx.GPIO_Conf = In_PullUp;
	uartRx.GPIO_Pin = RxPin_UART1;
	MyGPIO_Init ( &uartRx );
	MyUSART_Init (&muart); }

//Envoyer l'angle d'ouverture chaque 3 secondes 
void CommunicDist_Send(int angle){
	if (CommunicDist_FaibleBatterie(ref) == 1){
		int i;
		for (i=0; i<15;i++){
			MyUSART_Send_Mess(muart.Uart, bat_faible[i]);
			while(MyUSART_WaitOnSend(muart));
			}
		MyUSART_Send_Mess(muart.Uart, '\n');
		while(MyUSART_WaitOnSend(muart));
	}
	else {
		int i;
		for (i=0; i<7;i++){
			MyUSART_Send_Mess(muart.Uart, msg_angle[i]);
			while(MyUSART_WaitOnSend(muart));
		}
		for (i=0; i<3;i++){
			MyUSART_Send_Mess(muart.Uart, toArray(angle)[i]);
			while(MyUSART_WaitOnSend(muart));
		}
		MyUSART_Send_Mess(muart.Uart, '\n');
		while((muart.Uart->SR & USART_SR_TXE) ==0);
	}
}

//Envoyer des alertes si batterie faible ou limite du roulis
void CommunicDist_AlertSend(void){
		int i;
		for (i=0; i<19;i++){
			MyUSART_Send_Mess(muart.Uart, msg_alerte[i]);
			while(MyUSART_WaitOnSend(muart));
			}
		MyUSART_Send_Mess(muart.Uart, '\n');
		while(MyUSART_WaitOnSend(muart));
}