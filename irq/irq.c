#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/* Enable interrupts */
//#define sei()

/* 7th bit of status reg is interrupt enable */
//#define INT_DISABLE() SREG &= ~(1<<7)
//#define INT_ENABLE() SREG |= (1 << 7)

/* Service Routine for button press on pin 14 */
ISR(PCINT0_vect)
{
	/* Toggle pin 13 */
	PORTD ^= (1 << 7);
	//PORTD |= (1 << 7);

	/* Reenable interrupts */
	//INT_ENABLE();
}

int main()
{
	//EIMSK |= 1<<1;
	//INT_ENABLE();
	sei();
	PCICR |= 1;

	/* Pin Change Mask Register sets individual pin interrupts, turn on pin 14's */
	PCMSK0 |= 1;

	DDRB &= ~1; // Make pin 14 be an input
	DDRD |= (1 << 7);    // Make pin 13 be an output.  
	while(1)
	{
		//PORTD |= (1 << 7);   // Turn the LED on.
		//_delay_ms(500);
		//PORTD &= ~(1 << 7);  // Turn the LED off.
		//_delay_ms(500);
	}
}

