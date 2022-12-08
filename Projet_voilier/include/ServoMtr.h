#ifndef SERVOMTR_H
#define SERVOMTR_H

#include "MYTIMER.h"
#include "MYGPIO.h"



//Initialisation du Servomoteur
void Init_SERVOMTR(void); 


//Calcul du rapport cyclique pour varier l'angle de rotation du servo moteur
int Calcul_DutyCycle_SERVOMTR(int Angle_Vent);



#endif
