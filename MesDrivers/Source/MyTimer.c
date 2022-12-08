#include "stm32f10x.h"
#include "MyTimer.h"

void (*p_function)(void);

void MyTimer_Base_Init( MyTimer_Struct_TypeDef * Timer )
{
	//on branche le timer
	
		if(Timer->Timer==TIM1)RCC->APB2ENR |=RCC_APB2ENR_TIM1EN;
		else if (Timer->Timer==TIM2) RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
		else if (Timer->Timer==TIM3) RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
		else RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
							
	//on fixe la période demandée
	Timer->Timer->ARR=Timer->ARR;
	Timer->Timer->PSC=Timer->PSC;
	//on active le compteur
	Timer->Timer->CR1|=0x01;
}


void MyTimer_Active_IT ( TIM_TypeDef * Timer , char Prio,void (*IT_function)(void)) 
{
	Timer->DIER|=0x01;//demande d'interruption au debordement de timer
	if (Timer==TIM1)
	{		NVIC_EnableIRQ((IRQn_Type)(25));
			NVIC_SetPriority((IRQn_Type)(25),Prio);
	}
	else if (Timer==TIM2) 
	{
		NVIC_EnableIRQ(28);
		NVIC_SetPriority(28,Prio);
	}
	else if (Timer==TIM3)
	{
		NVIC_EnableIRQ(29);
		NVIC_SetPriority(29,Prio);
	}
	else 
	{
		NVIC_EnableIRQ(30);
		NVIC_SetPriority(30,Prio);
		
	}
	
	p_function= IT_function;
	
}	

void TIM1_UP_IRQHandler(void)
{
	(*p_function) ();
	TIM1->SR&= ~(0x01);//remet le bit activé à 0
}

void TIM2_IRQHandler(void)
{
	(*p_function) ();
	TIM2->SR&= ~(0x01);//remet le bit activé à 0
}

void TIM3_IRQHandler(void)
{
	(*p_function) ();
	TIM3->SR&= ~(0x01);//remet le bit activé à 0
}

void TIM4_IRQHandler(void)
{
	(*p_function) ();
	TIM4->SR&= ~(0x01);//remet le bit activé à 0
}


//	Initialisation PWM du Timer
void Timer_PWM_Init(TIM_TypeDef *Timer, int Channel){
	 
	switch (Channel) {
		
			 case 1:
			 	// On utilise le PWM en mode 1 
			  Timer->CCMR1 &= ~(6 << 4);  
			  Timer->CCMR1 |= (6 << 4);  //  CCMR1-> OC1M = 110
				Timer->CCER &= ~(0 << 0);
				Timer->CCER |= (1 << 0);  // Open channel 1, active OC1
		    //Initialisation de la PWM d'un Duty Cycle de 0%
				Timer->CCR1=0;  // Reset CCR1
				break;
			 
			 case 2:
				 
			  Timer->CCMR1 &= ~(6 << 12); 
			  Timer->CCMR1 |= (6 << 12);  //  CCMR1-> OC2M = 110
				Timer->CCER &= ~(0 << 4);
				Timer->CCER |= (1 << 4); // Open channel 2, active OC2
		    
				Timer->CCR2=0; // Reset CCR2
				break;
			 
			 case 3:

			  Timer->CCMR2 &= ~(6 << 4); 
			  Timer->CCMR2 |= (6 << 4);  //  CCMR2-> OC1M = 110
				Timer->CCER &= ~(0 << 8);
				Timer->CCER |= (1 << 8); // Open channel 3, active OC3

				Timer->CCR3=0; // Reset CCR3
				break;
			 
			 case 4:

			  Timer->CCMR2 &= ~(6 << 12); 
			  Timer->CCMR2 |= (6 << 12); //  CCMR2-> OC2M = 110
				Timer->CCER &= ~(0 << 12);
				Timer->CCER |= (1 << 12); // Open channel 4, active OC4

				Timer->CCR4=0; // Reset CCR4
				break;
			 
			 default:

				 break;
		 
 }
	// Enable bit MOE 
	Timer->BDTR |= (0x1 << 15);
	
}
//	 Fixer le rapport cyclique de la PWM 
void Timer_PWM_DutyCycle_Fix(TIM_TypeDef *Timer, int Duty_Cycle, int Channel){
     int Arr;
     if ((Duty_Cycle <= 100) && (Duty_Cycle >= 0)) {
			 
       Arr = Timer->ARR;
			 
			 switch (Channel) {
				 case 1:
                     Timer->CCR1 = (Arr*Duty_Cycle)/100;
				     break;

				 case 2:
                     Timer->CCR2 = (Arr*Duty_Cycle)/100;
				     break;

				 case 3:
                     Timer->CCR3 = (Arr*Duty_Cycle)/100;
				     break;

				 case 4:
                     Timer->CCR4 = (Arr*Duty_Cycle)/100;
				     break;
			 }
     }
 }
