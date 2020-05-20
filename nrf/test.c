#include "nrf.h"

const nrf_state_t MODE = tx;

int main()
{
	DDRD |= (1 << 7);    // Make pin 13 be an output
	DDRB &= ~(1 << 0);   // pin 14 input
	SPI_MasterInit();
	nrfSetup();
	nrfSetMode(MODE);

	blinkBinary(nrfReadReg(NRF_CONFIG));

	if(MODE == tx)
	{
	uint8_t data[2] = {170, 85};
	while(1)
	{
		nrfTransmit(data, 2);
		blinkBinary(nrfNop());
		_delay_ms(12000);
		//PORTD ^= (1<<7);
	}
	}
	else
	{
	uint8_t data[2] = {0,255};
	while(1)
	{
		uint8_t irqPin = PINB & (1<<0);
		if(irqPin == 0)
		{
			nrfReceive(data, 2);
			//blinkBinary(data[0]);
			blinkBinary(data[1]);
			nrfWriteReg(NRF_STATUS, 255); // clear interrupt
			nrfWriteReg(NRF_STATUS, 0);
		}
	}
	}
}
