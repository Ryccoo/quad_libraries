#include "Arduino.h"
#include <inttypes.h>

// Hardware limitations information
#define MIN_SERVO_PULSE_WIDTH 		544
#define MAX_SERVO_PULSE_WIDTH 		2400
#define DEFAULT_SERVO_PULSE_WIDTH   1500
#define REFRESH_INTERVAL 		    20000

// Aliases for timer config and loading
#define SERVO_TIMER				TIMER2_BASE
#define SERVO_TIME_CFG			TIMER_CFG_PERIODIC
#define SERVO_TIMER_TRIGGER		TIMER_TIMA_TIMEOUT
#define SERVO_TIMER_INTERRUPT	INT_TIMER2A
#define SERVO_TIMER_A			TIMER_A
#define SERVO_TIMER_PERIPH		SYSCTL_PERIPH_TIMER2

// Other defines
#define SERVOS_PER_TIMER 	8
#define INVALID_SERVO_PIN	255

#ifndef SERVO_H
#define SERVO_H

class ServoClass {
private:
	volatile unsigned long g_ulPeriod;
	volatile unsigned long g_ulTicksPerMicrosecond;  // Holds the calculated value
	uint8_t g_ulServoPins[SERVOS_PER_TIMER + 1];
	unsigned long g_ulServoPulse[SERVOS_PER_TIMER + 1];
	volatile uint8_t g_iServoNo;
	volatile unsigned long g_ulPulseWidth;
	void setRefresh(void);

public:
	ServoClass(void);
	void attach(uint8_t index, uint8_t pin);
	void detach(uint8_t index);
	void writeMicroseconds(uint8_t index, unsigned long value);

	// Stellaris Interrupt Service Routine
	void ServoIntHandler(void);
};

extern ServoClass Servo;
extern "C" void ServoIntHandler(void);
#endif
