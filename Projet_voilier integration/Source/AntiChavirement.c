#include "stdio.h"
#include "Antichavirement.h"
#include "MyGPIO.h"

void GPIO_Config (void)
{

	
	MyGPIO_Struct_TypeDef NSS={GPIOA,4,Out_Ppull};
	MyGPIO_Struct_TypeDef SCK={GPIOA,5,AltOut_Ppull};
	MyGPIO_Struct_TypeDef MISO={GPIOA,6,In_Floating};
	MyGPIO_Struct_TypeDef MOSI={GPIOA,7,AltOut_Ppull};

	RCC->APB2ENR |= (1 << 2);
	
	MyGPIO_Init(&NSS);
	MyGPIO_Init(&SCK);
	MyGPIO_Init(&MISO);
	MyGPIO_Init(&MOSI);
}
	

void SPI_Enable (void)
{
	SPI1->CR1 |= (SPI_CR1_SPE);   // SPE=1, Peripheral enabled
}

void SPI_Disable (void)
{
	SPI1->CR1 &= ~(SPI_CR1_SPE);   // SPE=0, Peripheral Disabled
}

void CS_Enable (void)
{
	GPIOA->BSRR |= (1<<4)<<16;
}

void CS_Disable (void)
{
	GPIOA->BSRR |= (1<<4);
}

void SPI_Config(void){
  RCC->APB2ENR |= (RCC_APB2ENR_SPI1EN);  // Enable SPI1 CLock
	
  SPI1->CR1 |= SPI_CR1_CPOL| SPI_CR1_CPHA;   // CPOL=1, CPHA=1
	
  SPI1->CR1 |= (SPI_CR1_MSTR);  // Master Mode
	
  SPI1->CR1 |= (SPI_CR1_BR_0) | (SPI_CR1_BR_1) | (SPI_CR1_BR_2);  // BR[2:0] = 111: fPCLK/256, PCLK2 = 72MHz, SPI CLK = 281,25kHz
	
  SPI1->CR1 &= ~(SPI_CR1_LSBFIRST);  // LSBFIRST = 0, MSB first
	
  SPI1->CR1 |= (SPI_CR1_SSM) | (SPI_CR1_SSI);  // SSM=1, SSI=1 -> Software Slave Management
	
  SPI1->CR1 &= ~(SPI_CR1_RXONLY);  // RXONLY = 0, full-duplex
	
  SPI1->CR1 &= ~(SPI_CR1_DFF);  // DFF=0, 8 bit data
	
  SPI1->CR2 = 0; // Reset CR2 Register

}

void SPI_Transmission(uint8_t *data, int size){
	uint8_t clear; //unsigned char
	//Il faut vérifier l’état du flag TxE puis écrire la données dans DR //
	int i=0;
	while (i<size)
	{
		while (!((SPI1->SR)&(SPI_SR_TXE))){};  //indique si le buffer est vide
		SPI1->DR=data[i];
		i++;
	}
	
	while (!((SPI1->SR)&(SPI_SR_TXE))){};  //indique si le buffer est vide

	while (((SPI1->SR)&(SPI_SR_BSY))){}; //indique que le buffer n'est pas en communication

	
	clear= SPI1->DR; // vide le Overrun flag en lisant les donnees sur DR et SR
	clear= SPI1->SR;
}

void SPI_Receive (uint8_t *data,int size)
{
	while (size)
	{
		while (((SPI1->SR)&(SPI_SR_BSY))) {};  // indique que le buffer n'est pas en communication
		SPI1->DR = 0;  // envoi d'une info de test
		while (!((SPI1->SR) &(SPI_SR_RXNE))){};
		// indique que le buffer n'est pas vide
		*data++= (SPI1->DR);
		size--;
	}
}

void adxl345_write (uint8_t address, uint8_t value)
{
	uint8_t data[2];
	data[0] = address|0x40;  // multibyte write
	data[1] = value;
	CS_Enable ();  // pull the cs pin low
	SPI_Transmission (data,2);  // write data to register
	CS_Disable ();  // pull the cs pin high
}
	

void adxl345_read (uint8_t address, uint8_t *RxData)
{
	address |= 0x80;  // read operation
	address |= 0x40;  // multibyte read
	CS_Enable ();  // pull the pin low
	SPI_Transmission (&address,1);  // send address
	SPI_Receive (RxData,6);  // receive 6 bytes data
	CS_Disable ();;  // pull the pin high
}

void adxl345_init (void)
{
	adxl345_write (DATA_FORMAT, 0x01);  // data_format range= +- 4g
	adxl345_write (POWER_CTL, 0x00);  // reset all bits
	adxl345_write (POWER_CTL, 0x08);  // power_cntl measure and wake up 8hz
}

void Delay_us (uint16_t us){

	int i=0;
 	while (i<us){
		if (TIM3->SR & TIM_SR_UIF) { // Scrutation de UIF
				TIM3->SR = TIM3->SR & ~TIM_SR_UIF; // Remise à zéro de UIF
				i++;
		}
	}
}
	
void Delay_ms (uint16_t ms){
	int i;
	for (i=0;i<ms;i++){
		Delay_us(1000); //attendre 1ms
	}
}


