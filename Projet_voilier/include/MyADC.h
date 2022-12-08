
#ifndef MYADC_H
#define MYADC_H
#include "stm32f10x.h"

//Initialisation de l'ADC à une même fréquence d'échantillonage pour tout ses channels
float Init_ADC_Single_Conv(ADC_TypeDef * ADC, u16 Duree_Ech_us);
void ADC_Start(ADC_TypeDef * ADC, char Channel);

#endif


/*#ifndef ADC_H
#define ADC_H

#include "stm32f10x.h"


//Initialisation ADC
void MyADC_Init (ADC_TypeDef * ADC);


//Configuration ADC
void Conf_ADC(ADC_TypeDef * ADC, u8 Channel);


// Valeur retournee par l'ADC
int Valeur_ADC(ADC_TypeDef * ADC);


#endif*/
