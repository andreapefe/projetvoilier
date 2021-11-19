#include "BATTERIE.h"
#include "MyGPIO.h"
#include <math.h>

void MyBatterie_Init(void) 
{
	//On configure PB0 en mode analog input
	MyGPIO_Struct_TypeDef GPIO_Batterie={GPIOB,0,In_analog};
	MyGPIO_Init(&GPIO_Batterie);

	//Initialisation de l'adc
	MyADC_Init(ADC1);
}

int ADC_Batterie_Charge(void) { 
	//Conversion sur channel 9
	Config_ADC(ADC1, 9);
	
	return Val_ADC(ADC1);
}


uint16_t Tension_Batt(void) {
	

	float Pont_Diviseur = 1/13.; // cahier des charges
	float Resolution_ADC = 4096/3.3; // 12-bit resolution ==> 2^12=4096
	// Tension de batterie ==> pont diviseur de tension avec un facteur de 1/13

	unsigned int Tension_Batterie = (float)( (float)(ADC_Batterie_Charge()) / (Resolution_ADC * Pont_Diviseur));
	
	return Tension_Batterie+1;

	
}


uint16_t Pourcentage_Batt(void) {
	
	float Alim = 12.0;
	// Pourcentage de la batterie 
	
	uint16_t Pourcentage_Batterie = (float) ((float)(Tension_Batt()) / Alim)*100;
	
	return Pourcentage_Batterie;

	
}

int intToAscii(int number) {
   return '0' + number;
}

int* ascii_to_numbers(int pourcentage){
	static int traduction_ascii[3];
	if (pourcentage==100){
		traduction_ascii[0]=intToAscii(0); //unite
		traduction_ascii[1]=intToAscii(0); //dizaine
		traduction_ascii[2]=intToAscii(1); //centaine
		
	}else{
		traduction_ascii[0]=intToAscii(pourcentage%10); //unite
		traduction_ascii[1]=intToAscii(floor(pourcentage/10));	//dizaine
	}

	
	return traduction_ascii;
};