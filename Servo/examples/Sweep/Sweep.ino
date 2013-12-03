#include <Servo.h>
#include <Energia.h>

void setup(){
	// Associating an index with a pin. There are 8 indexs, from 0 to
	// 7, to use them you need to associate the index with the desired
	// output pin. Once an index has an associated pin, the desired
	// microseconds for an index value will be passed to the
	// previously associated pin
	Servo.attach(0, PF_1);
	Servo.attach(1, PF_2);
}

void loop(){
	for(int k = 0; k <= 10; k++)
	{
		Servo.writeMicroseconds(0, 1000 + k * 1000 / 10);
		Servo.writeMicroseconds(1, 1000 + k * 1000 / 10);
		delay(100);
	}
	for(int k = 10; k >= 0; k--)
	{
		Servo.writeMicroseconds(0, 1000 + k * 1000 / 10);
		Servo.writeMicroseconds(1, 1000 + k * 1000 / 10);
		delay(100);
	}
}
