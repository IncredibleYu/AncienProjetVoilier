#ifndef COMMUNICDIST_H
#define COMMUNICDIST_H
#include "MyUSART.h"

//Initialisation l'Adc pour le niveau de batterie
void CommunicDist_InitADC(void);

//Envoyer le niveau de la batterie
int CommunicDist_NiveauBat(void);

//PourBatFaible pourcentage de la batterie
//Envoyer 0 si Batterie "pleine" et 1 si batterie faible
int CommunicDist_FaibleBatterie(char PourBatFaible);

//Initialisation de l'UART
void CommunicDist_InitUart(void);

//Envoyer l'angle d'ouverture chaque 3 secondes 
void CommunicDist_Send(int angle);

//Envoyer des alertes si batterie faible ou limite du roulis 
void CommunicDist_AlertSend(void);
#endif