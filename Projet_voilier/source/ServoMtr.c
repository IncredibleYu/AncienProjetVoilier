#include "SERVOMTR.h"



//Initialisation du Servomoteur
// Sortie PWM -> PA6
//PA6 -> Output Alternate function push-pull
void Init_SERVOMTR(void){
		 MyGPIO_Struct_TypeDef GPA6 = {GPIOA,6,AltOut_Ppull};
     MyGPIO_Init(&GPA6);
		 RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
		 TIM3->CR1|=0x01;
     Timer_PWM_Init(TIM3,1);
     
	   
}




//Calcul du rapport cyclique pour varier l'angle de rotation du Servomoteur
int Calcul_DutyCycle_SERVOMTR(int Angle_Vent) {

	int Duty_Cycle;
	

	if (Angle_Vent <= 45)  {
        Duty_Cycle = 5;
   }
   if (Angle_Vent > 45 && Angle_Vent < 68) {
		Duty_Cycle = 6; 
	}
	if (Angle_Vent >= 68 && Angle_Vent < 90) {
		Duty_Cycle = 7; 
	}
	if (Angle_Vent >= 90 && Angle_Vent < 113) {
		Duty_Cycle = 8; 
	}
   if (Angle_Vent >= 113 && Angle_Vent < 135) {
		Duty_Cycle = 9; 
	}
	if (Angle_Vent >= 135 && Angle_Vent < 225) {
		Duty_Cycle = 10; 
	}
	if (Angle_Vent >= 225 && Angle_Vent < 248) {
		Duty_Cycle = 9; 
	}
	if (Angle_Vent >= 248 && Angle_Vent < 270) {
		Duty_Cycle = 8; 
	}
   if (Angle_Vent >= 270 && Angle_Vent < 293) {
		Duty_Cycle = 7; 
	}
	if (Angle_Vent >= 293 && Angle_Vent < 315) {
		Duty_Cycle = 6; 
	}
	if (Angle_Vent >= 315)  {
        Duty_Cycle = 5;
   }
	
	
	return Duty_Cycle;
}


