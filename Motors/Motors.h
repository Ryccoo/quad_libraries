#ifndef _Motors_h
#define _Motors_h

#include "../Servo/Servo.h"


#define REARLEFTMOTOR   31
#define REARRIGHTMOTOR  32
#define FRONTLEFTMOTOR  33
#define FRONTRIGHTMOTOR 34

#define REARLEFT   0
#define REARRIGHT  1
#define FRONTLEFT  2
#define FRONTRIGHT 3

class Motors {
  public:
    Motors() {
      rearLeft = 1000;
      rearRight = 1000;
      frontLeft = 1000;
      frontRight = 1000;
    }
    void setValuesFront(int left, int right) {
      frontLeft = left;
      frontRight = right;
    }
    void setValuesRear(int left, int right) {
      rearRight = right;
      rearLeft = left;
    }
    void setValues(int fleft, int fright, int rleft, int rright) {
      setValuesFront(fleft, fright);
      setValuesRear(rleft, rright);
    }
    void write() {
      Servo.writeMicroseconds(REARLEFT,1000);
      Servo.writeMicroseconds(REARRIGHT,1000);
      Servo.writeMicroseconds(FRONTLEFT,1000);
      Servo.writeMicroseconds(FRONTRIGHT,1000);
    }
    void attach() {
      Servo.attach(REARLEFT,REARLEFTMOTOR);
      Servo.attach(REARRIGHT,REARRIGHTMOTOR);
      Servo.attach(FRONTLEFT,FRONTLEFTMOTOR);
      Servo.attach(FRONTRIGHT,FRONTRIGHTMOTOR);
    }
    int getFrontLeft() {
      return frontLeft;
    }
    int getFrontRight() {
      return frontRight;
    }
    int getRearLeft() {
      return rearLeft;
    }
    int getRearRight() {
      return rearRight;
    }
  private:
    int rearLeft;
    int rearRight;
    int frontLeft;
    int frontRight;
};

Motors Rotors;

#endif
