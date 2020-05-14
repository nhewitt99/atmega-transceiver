#define MODE tx

#define F_CPU 1000000UL // 1 MHz
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include "nrf.h"

// spi defines
#define PORT_SPI PORTB
#define DDR_SPI DDRB
#define DD_MOSI 3
#define DD_MISO 4
#define DD_SCK 5
#define DD_SS 2
#define CPOL 3
#define CPHA 2

// nrf ce pin defines
#define PORT_CE PORTB
#define DDR_CE DDRB
#define DD_CE 1

void SPI_MasterInit(void)
{
	/* Set MOSI, SCK, SS output, others input */
	DDR_SPI = (1<<DD_MOSI) | (1<<DD_SCK) | (1<<DD_SS);
	/* Enable SPI, master, set clock rate fck/16 */
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0);
	// Note: SPI mode will be zero
}

uint8_t SPI_MasterTransmit(uint8_t cData)
{
	/* Write byte to SPI data register */
	SPDR = cData;
	/* Wait for tx to end (idk why this works) */
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}

/*  Writes a value to a register on the nrf chip
 *  reg: register to write to
 *  data: value to put in register
 *  return: STATUS register
 */
uint8_t nrfWriteReg(uint8_t reg, uint8_t data)
{
	uint8_t command = NRF_MASK_W_REGISTER | reg;

	// high-to-low signals a command
	PORT_SPI &= ~(1<<DD_SS);

	uint8_t status = SPI_MasterTransmit(command);
	SPI_MasterTransmit(data);

	PORT_SPI |= (1<<DD_SS);
	return status;
}

/*  Reads a value from a register on the nrf chip
 *  reg: register to read from
 *  return: register's value
 */
uint8_t nrfReadReg(uint8_t reg)
{
	uint8_t command = NRF_MASK_R_REGISTER | reg;

	// high-to-low signals a command
	PORT_SPI &= ~(1<<DD_SS);

	SPI_MasterTransmit(command);
	uint8_t data = SPI_MasterTransmit(0);

	PORT_SPI |= (1<<DD_SS);
	return data;
}

uint8_t nrfTransmit(uint8_t* data, uint8_t len)
{
	// high-to-low signals a command
	PORT_SPI &= ~(1<<DD_SS);

	uint8_t status = SPI_MasterTransmit(NRF_W_TX_PAYLOAD);

	for(uint8_t i = 0; i < len; i++)
		SPI_MasterTransmit(*(data + 8*i));

	PORT_SPI |= (1<<DD_SS);
	return status;
}

/*  Powers up nrf chip
 *  Simultaneously sets config bits
 *  return: true if error
 */
bool nrfPowerUp(void)
{
	// allow rx interrupts,
	// enable crc checks,
	// power up,
	// set to rx mode
	uint8_t config = 0 | (1<<NRF_CONFIG_MASK_TX_DS)
			| (1<<NRF_CONFIG_MASK_MAX_RT)
			| (1<<NRF_CONFIG_EN_CRC)
			| (1<<NRF_CONFIG_PWR_UP)
			| (1<<NRF_CONFIG_PRIM_RX);
	uint8_t status = nrfWriteReg(NRF_CONFIG, config);

	return status == 0;
}

bool nrfSetupRF(void)
{
	// set 250kbps data rate,
	// set redundant 250kbps bits,
	// set -12dBm tx power
	uint8_t setup = 0 | (1<<NRF_RF_SETUP_RF_DR_LOW)
			| (0<<NRF_RF_SETUP_RF_DR_HIGH)
			| (0b01<<NRF_RF_SETUP_RF_PWR);
	uint8_t status = nrfWriteReg(NRF_RF_SETUP, setup);

	return status == 0;
}

bool nrfSetChannel(uint8_t channel)
{
	if(channel > 125) return true;

	uint8_t status = nrfWriteReg(NRF_RF_CH, channel);

	return status == 0;
}

void nrfSetup(void)
{
	// turn on
	nrfPowerUp();
	_delay_ms(5);

	// set power to low (or high?) and low speed
	nrfSetupRF();

	/* no need to set rx/tx addresses or
	 * enable/disable data pipes, the reset
	 * values of these registers are fine.
	 * default addr: E7:E7:E7:E7:E7
	 */

	// select channel 108
	nrfSetChannel(108);

	// set payload size
	nrfWriteReg(NRF_RX_PW_P0, 2);

	// enable chip to start rx mode
	DDR_CE |= (1<<DD_CE);
	PORT_CE |= (1<<DD_CE);
}

void nrfSetMode(nrf_state state)
{
	// read config
	uint8_t config = nrfReadReg(NRF_CONFIG);

	// mask for PRIM_RX bit
	uint8_t primrx = (1<<NRF_CONFIG_PRIM_RX) & config;
	primrx = (primrx>>NRF_CONFIG_PRIM_RX);

	// change mode if necessary
	if((uint8_t)state != primrx)
	{
		config ^= (1<<NRF_CONFIG_PRIM_RX);

		// disable chip, update config, re-enable
		PORT_CE &= ~(1<<DD_CE);
		nrfWriteReg(NRF_CONFIG, config);
		PORT_CE |= (1<<DD_CE);

		// 150us delay to set mode, plus some wiggle room
		_delay_us(200);
	}
}

void blinkBinary(uint8_t data)
{
	for(int i = 7; i >= 0; i--)
	{
		PORTD |= (1<<7);
		if(data & (1<<i))
		{
			_delay_ms(750);
			PORTD &= ~(1<<7);
			_delay_ms(250);
		}
		else
		{
			_delay_ms(250);
			PORTD &= ~(1<<7);
			_delay_ms(750);
		}
	}
}

int main()
{
	DDRD |= (1 << 7);    // Make pin 13 be an output.
	SPI_MasterInit();
	nrfSetup();
	nrfSetMode(MODE);
	uint8_t config, rf, channel;

	if(MODE == tx) {
		uint8_t test[2] = {170, 85};
		nrfTransmit(test, 2);
	}

	while(1)
	{
		config = nrfReadReg(NRF_CONFIG);
		rf = nrfReadReg(NRF_RF_SETUP);
		channel = nrfReadReg(NRF_RF_CH);

		blinkBinary(config);
		_delay_ms(1000);
		blinkBinary(rf);
		_delay_ms(1000);
		blinkBinary(channel);
		_delay_ms(1000);
	}
}
