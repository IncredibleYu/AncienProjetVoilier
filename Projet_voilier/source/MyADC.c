#include "MyADC.h"
#include "math.h"

float Init_ADC_Single_Conv(ADC_TypeDef * ADC, u16 Duree_Ech_us){
	int i;	 
	float SampleTimeSelection[8] = {0.0139*1.5,0.0139*7.5,0.0139*13.5,0.0139*28.5,0.0139*41.5,0.0139*55.5,0.0139*71.5,0.0139*239.5};
	float SampleTimeCalc[8] = {0.0139*1.5,0.0139*7.5,0.0139*13.5,0.0139*28.5,0.0139*41.5,0.0139*55.5,0.0139*71.5,0.0139*239.5};
	float CalcSampleTime = 10.0;
	char index_value;
	
	RCC->APB2ENR|=RCC_APB2ENR_ADC1EN;
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6;
	ADC->CR2 |= 0x01;
	ADC->CR2 &= ~ADC_CR2_CONT;
	ADC->CR2 |= ADC_CR2_EXTTRIG;  
	ADC->CR2 |= (ADC_CR2_EXTSEL_0 |ADC_CR2_EXTSEL_1 |ADC_CR2_EXTSEL_2);
			
			for (i = 0;i<8;i++){
				SampleTimeCalc[i] = fabs(SampleTimeCalc[i] - Duree_Ech_us);}

			for (i = 0;i<8;i++){
				if (SampleTimeCalc[i]<CalcSampleTime){
					index_value = i;
					CalcSampleTime = SampleTimeCalc[i];}
			}
			for (i = 0;i<10;i++){ 
			ADC->SMPR2 |= (index_value)<< 3*i;
			}
			for (i = 0;i<8;i++){ 
			ADC->SMPR1 |= (index_value)<< 3*i;
			}
			ADC->CR2 |= ADC_CR2_CAL;
			return SampleTimeSelection[index_value];
}

void ADC_Start(ADC_TypeDef * ADC, char Channel){
	ADC->SQR1 &= ~ADC_SQR1_L_0;
	ADC->SQR1 &= ~ADC_SQR1_L_1;
	ADC->SQR1 &= ~ADC_SQR1_L_2;
	if (Channel < 5 ){
		ADC->SQR3 |= Channel;
	}
	ADC->CR2 |=ADC_CR2_SWSTART;
}




//Initialisation ADC
/*void MyADC_Init (ADC_TypeDef * ADC) 
{
    
	if (ADC == ADC1) {
		(RCC->APB2ENR)|= RCC_APB2ENR_ADC1EN;
		//On divise par 6 la fréquence d'entrée (72MHz / 6 = 12MHz) car ADC doit avoir une fréquence < 14MHz pour que le périphérique fonctionne.
	   	 RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6;

		// On active ADC1 
		ADC1->CR2 |= (0x01 << 0);
	}
	if (ADC == ADC2) {
		(RCC->APB2ENR)|= RCC_APB2ENR_ADC2EN;
	    	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6;
		
		// On a active ADC2
		ADC2->CR2 |= (0x01 << 0);
	}
	
}


//Configuration ADC
void Conf_ADC(ADC_TypeDef * ADC, u8 Channel) 
{
    // Init EOC register (on le met à 0)
		ADC->SR &= ~(0x01 << 1);

    //On souhaite faire une seule conversion
    ADC->SQR1 &= ADC_SQR1_L; 

    //On choisit quelle voie on souhaite convertir
    ADC->SQR3 |= Channel;

}


// Valeur retournee par l'ADC
int Valeur_ADC(ADC_TypeDef * ADC) 
{
	//On lance la conversion
    ADC->CR2 |= (0x01 << 0); 
    
    //On attend que le flag EOC passe à 1
    while (!(ADC->SR & ADC_SR_EOC)){}
			
		// validation de la conversion
		ADC->SR &= ~ADC_SR_EOC; 
			
	 // retour de la conversion
		return ADC->DR & ~((0x0F) << 12);
}*/
