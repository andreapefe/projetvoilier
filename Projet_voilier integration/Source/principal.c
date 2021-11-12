#include "Driver_GPIO.h"
#include "stdlib.h"
#include "stm32f10x.h"
#include "MyTimer.h"
#include "AntiChavirement.h"
#include "math.h"


u16 compte;
uint8_t RxData[6];
uint8_t data_test1[1];
uint8_t data_test2[1];
uint8_t data_test3[1];
uint8_t data_test4[1];
uint8_t data_test5[1];
uint8_t data_test6[1];

uint16_t x,y,z;
float	x_acc,y_acc,z_acc;

int main(void)
	
{ 

	// CONFIG DES PINS DE LA SPI+TIMER+GPIO
	MyTimer_Struct_TypeDef T1={TIM1,71,0}; //config pour 1MHz soit 1us
	
	MyTimer_Base_Init(&T1);
	
	
	GPIO_Config();
	SPI_Config();
	SPI_Enable();
	adxl345_init();

	/*do{	
		
	// TEST //
	//SPI1->DR=0x00; //addresse à envoyer
	//while (!((SPI1->SR)&(SPI_SR_TXE))){};  //indique si le buffer de transmission est vide
	//while (!((SPI1->SR)&(SPI_SR_RXNE))){}; 		// indique que le buffer de reception n'est pas vide	
	//data_test[0]=SPI1->DR;
	// TEST //
		
	Delay_ms(500); // attendre 500ms
		
		
	}while(1);*/
	do{
		
		/*adxl345_read(0x32,data_test1);
		adxl345_read(0x33,data_test2);
		adxl345_read(0x34,data_test3);
		adxl345_read(0x35,data_test4);
		adxl345_read(0x36,data_test5);
		adxl345_read(0x37,data_test6);
		
		x = ((data_test2[0]<<8)|data_test1[0]); // DATA X0, X1
		y = ((data_test4[0]<<8)|data_test3[0]); // DATA Y0, Y1
		z = ((data_test6[0]<<8)|data_test5[0]); // DATA Z0, Z1*/
		
		/*adxl345_write(0x1E,0);
		adxl345_read(0x1E,data_test1);
		
		adxl345_write(0x1F,0);
		adxl345_read(0x1F,data_test2);
		
		adxl345_write(0x20,0);
		adxl345_read(0x20,data_test3);*/
		
		
	  adxl345_read(0x32,RxData);
		
		if (RxData[1] & (1<<8)){
			RxData[1]= RxData[1] & ~(1<<8);
		//	RxData[1]= -RxData[1];
		}
		
		if (RxData[3] & (1<<8)){
			RxData[3]= RxData[3] & ~(1<<8);
		//	RxData[3]= -RxData[3];
		}
				
		if (RxData[5] & (1<<8)){
			RxData[5]= RxData[5] & ~(1<<8);
		//	RxData[5]= -RxData[5];
		}
		
		x = ((RxData[1]<<8)|RxData[0]); // DATA X0, X1
		y = ((RxData[3]<<8)|RxData[2]); // DATA Y0, Y1
		z = ((RxData[5]<<8)|RxData[4]); // DATA Z0, Z1

		
		// Scale Factor for Xout, Yout and Zout is 7.8 mg/LSB for a +-4g, 10-bit resolution
		// ==> multiply by 0.0078 to get real acceleration values in g
		
		x_acc= x * 0.0078; 
		y_acc= y * 0.0078;
		z_acc= z * 0.0078;
		
		Delay_ms(500);
		
		if (acos(z_acc)*180/(atan(1)*4)>= 40){
				//relacher les voiles;
		}
		
	
	}while(1);
	

	


	}
