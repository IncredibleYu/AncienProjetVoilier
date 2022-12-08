#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "MyGPIO.h"
#include "MyTimer.h"
#include "ServoMtr.h"
#include "MySPI.h"
#include "CommunicDist.h"
#include "MyUSART.h"
#include "BATTRERIE.h"

#define ARR2 1439
#define PSC2 0
#define IDX 6
#define Arr 99 
#define Psc 14544 //EVENT_FREQUENCY =1440000 = ITRx_CLK / ( (PSC+1) * (ARR+1) * (RCR + 1))
#define SPI_CMD_MB 	0x40 //pour configurer la SPI en multiBytes
#define SPI_CMD_R		0x80 //pour configurer la SPI en mode Read
#define SPI_ADDR_POWER_CTRL	0x2D //l'adresse du registre POWER_CTL
#define SPI_ADDR_BW_RATE	0x2C //l'adresse du registre BW_RATE
#define SPI_ADDR_DATA_FORMAT	0x31 //l'adresse du registre DATA_FORMAT
#define SPI_ADDR_X0	0x32 //l'adresse pour lire la valeur X0
int res=0;
int dc;
int res2=0;

char ByteToSend=0;
char X0,X1,Y0,Y1,Z0,Z1;
int x,y,z; 
char dataFormat,Power_ctl;

//Le but de ce test est de la communication avec la télécommande est bidirectionnelle, 
//avec possibilité d'envoi de l'angle et de l'allure du bateau avec un délai de 3secondes.

MyGPIO_Struct_TypeDef GPIO_USART_RX = {GPIOA,10,In_PullDown};
USART_TypeDef * UART ;
MyUART_Struct_TypeDef * MyUart;
char testUart ;

void Callback() {
	testUart = MyUSART_Get_Byte(UART);
}

	
int main ( void )
{
	int test = 0;
	
	//configuration du tim2 pour le codeur incrémental
	
	//activation du timer2 en prenant 
  MyTimer_Struct_TypeDef MTIM2={TIM2,ARR2,PSC2};
	MyTimer_Struct_TypeDef TI = {TIM3,Arr,Psc};
	//activation des pins PA0 et PA1 du GPIOA 
	MyGPIO_Struct_TypeDef MGPIOA0={GPIOA,0,In_Floating};//PA0 en floating input
	MyGPIO_Struct_TypeDef MGPIOA1={GPIOA,1,In_Floating};//PA1 en floating input
	
	//on utilise le PC6 pour lire l'index du codeur incremental
	MyGPIO_Struct_TypeDef MGPIOC6={GPIOC,6,In_Floating};//PC6 en floating input
	
	MyTimer_Base_Init(&MTIM2);
	MyGPIO_Init (&MGPIOA0) ;//PA0 en floating input
	MyGPIO_Init (&MGPIOA1) ;//PA1 en floating input
	MyGPIO_Init (&MGPIOC6) ;//PC6 en floating input*/
	
	//On met TIM2_SMCR à 011 pour compter dans Tl1 ainsi que dans Tl2
	TIM2->SMCR &= ~(0x07);
	TIM2->SMCR |=(0x03);
	MyTimer_Base_Init(&TI);
	
	Init_SERVOMTR();
	

	
	while(test == 0){
		test = (GPIOC->IDR	& (0x01<<IDX));
	}
	
	TIM2->CNT=0;//on reset le CNT car on est à l'origine
 // on initialise la spi 2
	MySPI_Init(SPI2);
	MySPI_Set_NSS();
	
		//on fixe le bit measure de power control à 1
		//On lit la valeur dans POWER_CTL
	
	MySPI_Clear_NSS();
	MySPI_Send(SPI_CMD_R | SPI_ADDR_POWER_CTRL);
	Power_ctl=MySPI_Read();
	MySPI_Set_NSS();
		//on la modifie ensuite on l'envoie
	Power_ctl|=(0x01<<3);
	MySPI_Clear_NSS();
	MySPI_Send(SPI_ADDR_POWER_CTRL);
	MySPI_Send(Power_ctl);
	MySPI_Set_NSS();
	
	//on fixe le bit justify de DATA_FORMAT à 0
		//On lit la valeur dans DATA_FORMAT
	MySPI_Clear_NSS();
	MySPI_Send(SPI_CMD_R | SPI_ADDR_DATA_FORMAT);
	dataFormat=MySPI_Read();
	MySPI_Set_NSS();
		//on la modifie ensuite on l'envoie
	dataFormat &=~(0x01<<2);;
	MySPI_Clear_NSS();
	MySPI_Send(SPI_ADDR_POWER_CTRL);
	MySPI_Send(Power_ctl);
	MySPI_Set_NSS();
	
	UART=USART1 ;
	MyGPIO_Init(&GPIO_USART_RX);
	CommunicDist_InitADC();
	CommunicDist_InitUart();
	//MyUSART_Init (MyUart) ;
	//MyUSART_Active_IT (MyUart, Callback);
	MyUSART_Reception();
	MyUSART_Transmission();
	MyUSART_Send_Byte(UART, "quelque chose");

	while (1){
		res=(TIM2->CNT)/4;//on lit l'angle obtenu
		dc=Calcul_DutyCycle_SERVOMTR(res);
		Timer_PWM_DutyCycle_Fix(TIM3,dc,1);
		//print(&GPB4);
		res2=TIM3->CCR1;
		//partie SPI
		MySPI_Clear_NSS();//on branche le slave(ici la SPI)
		//On envoie un message pour dire qu'on lit à l'adresse à partir de X0
		//en mode multiBytes pour les lire en méme temps
		MySPI_Send(SPI_CMD_R|SPI_CMD_MB|SPI_ADDR_X0);
		X0=MySPI_Read();//on lit X0
		X1=MySPI_Read();//on lit X1
		x=(int)X1+2^(8)*(int)X0;
		Y0=MySPI_Read();//on lit Y0
		Y1=MySPI_Read();//on lit Y1
		y=(int)Y1+2^(8)*(int)Y0;
		Z0=MySPI_Read();//on lit Z0
		Z1=MySPI_Read();//on lit Z1
		z=(int)Z1+2^(8)*(int)Z0;
		MySPI_Set_NSS();
		
	}
	


}
