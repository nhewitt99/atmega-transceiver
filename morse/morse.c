#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "morse.h"

machineState state = standby1;

/*
 * Standby1 is reset state
 * Sleep until button pressed or packet rx'ed
 * If button pressed, txOn
 * If packet available, rx
 */
void f_standby1(void)
{
	// Check which interrupt caused wakeup
	// TODO: read nrf available pin and button pin

	// if button pressed
	if(1) {
		state = txOn;
		f_txOn();
	}
	// if packet available
	else if(1) {
		state = rx;
		f_rx();
	}

	// something went wrong
}

void f_standby2(void)
{
	// Check which interrupt caused wakeup
	// TODO: check timer int register and read button

	// if 45ms have passed
	if(1) {
		state = txOn;
		f_txOn();
	}
	// if button turned off
	else if(1) {
		state = txOff;
		f_txOff();
	}

	// something went wrong
}

void f_standby3(void)
{
	// Check which interrupt caused wakeup
	// TODO: check timer int register and read nrf pin

	// if 100ms have passed
	if(1) {
		state = ledOff;
		f_ledOff();
	}
	// if packet available
	else if(1) {
		state = rx;
		f_rx();
	}

	// something went wrong
}

void f_txOn(void)
{
	// transmit turn on command via nrf

	// loop for 5ms before entering sleep to
	//   catch quick button releases
	while(0) {
		// if button off
		if(1) {
			state = txOff;
			f_txOff();
		}
	}

	// configure interrupts for standby2
	// nrf pin off,
	// button pin on,
	// 45ms timer on

	state = standby2;

	// return to sleep
}

void f_txOff(void)
{
	// transmit off command via nrf

	// configure interrupts for standby1
	// nrf pin on,
	// button pin on,
	// timer off

	state = standby1;

	// return to sleep
}

/*
 * Packet available on nrf chip, receive and decode it
 */
void f_rx(void)
{
	// Read payload from nrf chip

	// If payload says on, ledOn
	if(1) {
		state = ledOn;
		f_ledOn();
	}
	// Else if off, ledOff
	else if(1) {
		state = ledOff;
		f_ledOff();
	}

	// something went wrong
}

void f_ledOn(void)
{
	// Turn on led

	// Configure interrupts for standby3
	// nrf available pin on,
	// 100ms timer interrupt on,
	// button pin off

	state = standby3;

	// No more calls to allow return to sleep
}

void f_ledOff(void)
{
	// Turn off led

	// Configure interrupts for standby1
	// button pin on,
	// nrf pin on,
	// timer off

	state = standby1;

	// No more calls to allow return to sleep
}

/*
 * Called on interrupts to break from
 * low-power state in standby states.
 * All other state transitions are handled
 * within function calls to standby states.
 */
void wakeup(void)
{
	switch(state) {
		case standby1:
			f_standby1();
			break;
		case standby2:
			f_standby2();
			break;
		case standby3:
			f_standby3();
			break;
		default:
			f_standby1(); // something went wrong
	}
}

int main()
{

}
