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
    void set_values_front(int left, int right) {
      frontLeft = left;
      frontRight = right;
    }
    void set_values_rear(int left, int right) {
      rearRight = right;
      rearLeft = left;
    }
    void set_values_left(int left, int right) {
      frontLeft = left;
      rearLeft = left;
    }
    void set_values_right(int left, int right) {
      rearRight = right;
      frontRight = right;
    }
    void set_values(int fleft, int fright, int rleft, int rright) {
      set_values_front(fleft, fright);
      set_values_rear(rleft, rright);
    }
    void write(int fl, int fr, int rl, int rr) {
      Servo.writeMicroseconds(REARLEFT,rl);
      Servo.writeMicroseconds(REARRIGHT,rr);
      Servo.writeMicroseconds(FRONTLEFT,fl);
      Servo.writeMicroseconds(FRONTRIGHT,fr);
    }
    void attach() {
      Servo.attach(REARLEFT,REARLEFTMOTOR);
      Servo.attach(REARRIGHT,REARRIGHTMOTOR);
      Servo.attach(FRONTLEFT,FRONTLEFTMOTOR);
      Servo.attach(FRONTRIGHT,FRONTRIGHTMOTOR);
    }
    int get_front_left() {
      return frontLeft;
    }
    int get_front_right() {
      return frontRight;
    }
    int get_rear_left() {
      return rearLeft;
    }
    int get_rear_right() {
      return rearRight;
    }
  private:
    int rearLeft;
    int rearRight;
    int frontLeft;
    int frontRight;
};

extern Motors Rotors;

#endif
