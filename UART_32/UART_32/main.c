/*
 * Firmware_Task
 *
 * Created: 20-02-2024 
 * Author : Vishnu V Nair
 */ 
  
#include <avr/io.h>
#include <util/delay.h>					
#include <stdlib.h>					//Header for dtostrf function
#include <string.h>

#define F_CPU 16000000UL
#define Baudrate 2400
#define UBRR ((F_CPU/(16 * Baudrate)) - 1)			//Baud rate register

//Function to initialize UART
void uart_init()
{
	UBRRL = UBRR;					//Set baud rate to Lower register
	UBRRH = (UBRR >> 8);				//Set baud rate to Higher register
	UCSRB |= (1 << TXEN) | (1 << RXEN);		//Transmitter and Receiver enable
	UCSRC |= (1 << UCSZ1) | (1 << UCSZ0);		//UART char size to 8-bit
}

//Function to transmit data
void transmit(const char data)
{
	while(!(UCSRA & (1 << UDRE)));			//Wait until transfer buffer is empty
	
	UDR = data;					//Transmit data
}

//Function to receive data
int receive()
{
	while(!(UCSRA & (1 << RXC)));			//Wait for data to be received
	
	return UDR;					//Return received data
}

//Function to calculate transmission speed
void trans_speed(const char *trans_data)
{
	char speed_str[20];			//Variable to store trans speed

	float data_bits = strlen(trans_data) * 8;		//No of bit in transmitted data
	float speed = (Baudrate / data_bits) * 1000;			//Calculate speed in BPS

	dtostrf(speed, 5, 2, speed_str);			//Convert string to decimal

	//Transmission message part
	const char *message = "\nTransmission speed (bps): ";
	for(int i = 0; i < strlen(message); i++)
	{
		transmit(message[i]);
	}

	//To transmit speed
	for(int i = 0; i < strlen(speed_str); i++)
	{
		transmit(speed_str[i]);
	}
	transmit('\n');
}

int main(void) 
{
	uart_init();			//Initializing UART

	char received_data[1000];	//Variable to store received data

	while(1)
	{	
		//To receive char one by one
		for(int i = 0; i < 1000 ; i++)
		{
			received_data[i] = receive();

			//If null char is received exit loop
			if(received_data[i] == '\0')
			{
				break;
			}

			transmit(received_data[i]);		//Transmit received data
		}		
		trans_speed(received_data);			//Transmit transmission speed
	}	
}




