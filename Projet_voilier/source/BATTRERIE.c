/*#include "BATTRERIE.h"


//Initialisation de la mesure de la tension de la batterie
void MyBatterie_Init(void) {

	int champ = 0, num_bit = 0;
	//On configure PB0 en mode analog input
	MyGPIO_Struct_TypeDef MyGPIO_b = {GPIOB,0,In_Analog};

	//On initialise l'ADC
	MyADC_Init(ADC1);

}


//Recuperation de la tension de la batterie à l'entrèe de l'ADC
int ADC_Batterie_Charge(void) { 

//On commence la conversion sur Channel 8
	Conf_ADC(ADC1, 8);
	
	return Valeur_ADC(ADC1);

}


//Recuperation de la tension de la batterie en VOLTS
unsigned int Batterie_Voltage(void) {
	
	const float Pont_Diviseur = 1./13.;
	const float Resolution_ADC = 4096./3.3;

	// Calcul de tension de la batterie
	unsigned int Tension_Batterie = (float)( (float)(ADC_Batterie_Charge()) / (Resolution_ADC * Pont_Diviseur));
	
	return Tension_Batterie+1;
	
}


//Calcul du pourcentage de la batterie restante
unsigned int Batterie_Percentage(void) {
	
	const float Alim = 12.0;
	//Calcul de tension de la batterie en pourcentage
	
	unsigned int Percentage_Batterie = (float) ((float)(Batterie_Voltage()) / Alim)*100;
	
	return Percentage_Batterie;
	
}
*/