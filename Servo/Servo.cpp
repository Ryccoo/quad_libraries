#include "Energia.h"

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include <stdio.h>
#include <stdlib.h>

#include "Servo.h"

void ServoClass::setRefresh(void)
{
	unsigned long period_servo_sum = 0;
	for (int il_iter = 0; il_iter < SERVOS_PER_TIMER; il_iter++)
		period_servo_sum += g_ulServoPulse[il_iter];
	g_ulServoPulse[SERVOS_PER_TIMER] = REFRESH_INTERVAL - period_servo_sum;
}

ServoClass::ServoClass()
{
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|
	                SYSCTL_OSC_MAIN);

	// Initialize variables of the class

	g_ulPeriod = 0;

	for(int il_iter = 0; il_iter < SERVOS_PER_TIMER; il_iter++)
	{
		g_ulServoPins[il_iter] = INVALID_SERVO_PIN;
		g_ulServoPulse[il_iter] = DEFAULT_SERVO_PULSE_WIDTH;
	}

	g_iServoNo = 0;
	g_ulPulseWidth = 0;
	g_ulTicksPerMicrosecond = 0;

	setRefresh();

	//	for(int il_iter = 0; il_iter < SERVOS_PER_TIMER; il_iter++)
	//	{
	//		if (g_ulServoPins[il_iter] != INVALID_SERVO_PIN)
	//		{
	//			pinMode(g_ulServoPins[il_iter], OUTPUT);
	//			digitalWrite(g_ulServoPins[il_iter], LOW);
	//		}
	//	}

	// Enable TIMER
	ROM_SysCtlPeripheralEnable(SERVO_TIMER_PERIPH);

	// Enable processor interrupts.
	ROM_IntMasterEnable();

	// Configure the TIMER
	ROM_TimerConfigure(SERVO_TIMER, SERVO_TIME_CFG);

	// Calculate the number of timer counts/microsecond
	g_ulTicksPerMicrosecond = ROM_SysCtlClockGet() / 1000000;
	g_ulPeriod = g_ulTicksPerMicrosecond * REFRESH_INTERVAL;   // 20ms = Standard Servo refresh delay

	// Initially load the timer with 20ms interval time
	ROM_TimerLoadSet(SERVO_TIMER, SERVO_TIMER_A, g_ulPeriod);

	// Setup the interrupt for the TIMER1A timeout.
	ROM_IntEnable(SERVO_TIMER_INTERRUPT);
	ROM_TimerIntEnable(SERVO_TIMER, SERVO_TIMER_TRIGGER);

	// Enable the timer.
	ROM_TimerEnable(SERVO_TIMER, SERVO_TIMER_A);
}

void ServoClass::writeMicroseconds(uint8_t index, unsigned long value)
{
	if ((MIN_SERVO_PULSE_WIDTH <= value) && (value <= MAX_SERVO_PULSE_WIDTH) && (index < SERVOS_PER_TIMER))
		g_ulServoPulse[index] = value;

	setRefresh();
}

void ServoClass::ServoIntHandler(void)
{
	// Clear the timer interrupt.
	ROM_TimerIntClear(SERVO_TIMER, SERVO_TIMER_TRIGGER);

	// Get the pulse width value for the current servo from the array
	// if we have already serviced all servos (g_iServoNo = MAX_SERVO_NO)
	// then this value should be the 20ms period value
	unsigned long l_ulPulseWidth = g_ulTicksPerMicrosecond * g_ulServoPulse[g_iServoNo];

	// Re-Load the timer with the new pulse width count value
	ROM_TimerLoadSet(SERVO_TIMER, SERVO_TIMER_A, l_ulPulseWidth);

	// End the servo pulse set previously (if any)
	if(g_iServoNo > 0)  // If not the 1st Servo....
	{
		if (g_ulServoPins[g_iServoNo - 1] != INVALID_SERVO_PIN)
			digitalWrite(g_ulServoPins[g_iServoNo - 1], LOW);
	}

	// Set the current servo pin HIGH
	if(g_iServoNo < SERVOS_PER_TIMER)
	{
		if (g_ulServoPins[g_iServoNo] != INVALID_SERVO_PIN)
			digitalWrite(g_ulServoPins[g_iServoNo], HIGH);
		g_iServoNo++;  // Advance to next servo for processing next time
	}
	else
	{
		g_iServoNo = 0; // Start all over again
	}
}

void ServoClass::attach(uint8_t index, uint8_t pin)
{
	if ((index < SERVOS_PER_TIMER) && (pin != INVALID_SERVO_PIN))
	{
		g_ulServoPins[index] = pin;
		pinMode(pin, OUTPUT);
		digitalWrite(pin, LOW);
	}
}

void ServoClass::detach(uint8_t index)
{
	if (index < SERVOS_PER_TIMER)
	{
		digitalWrite(g_ulServoPins[index], LOW);
		g_ulServoPins[index] = INVALID_SERVO_PIN;
		g_ulServoPulse[index] = DEFAULT_SERVO_PULSE_WIDTH;

		setRefresh();
	}
}

void ServoIntHandler(void)
{
	Servo.ServoIntHandler();
}

ServoClass Servo;
