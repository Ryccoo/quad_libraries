#ifndef _Communication_h
#define _Communication_h

#include "Energia.h"
#include "../AccelGyro/AccelGyro.h"
#include "../ExternDef.h"
#include "../Motors/Motors.h"

// Communication constants
#define CONNECTIONSTATUS  42
#define SWITCHPOWER       255
#define ADDTOFRONTMOTORS  100
#define STOPALLMOTORS     3
#define SLOWALLMOTORS     5


class Communication {
  public:
    Communication() {};

    void init() {
      Serial1.begin(9600);
      terminal_mode = false;
      connectionActive = true;
    }

    void send(char* text) {
      Serial1.println(text);
    };

    void check_connection() {
      Serial1.print("#CON:");
      Serial1.println("Test for reply");
      if (lastConnectionTime - millis() > 3000) {
        Rotors.set_values(1000, 1000, 1000, 1000);
        connectionActive = false;
      }
      lastConnectionTime = millis();
    }

    void receive() {
      if (Serial1.available() > 0 ) {
        //  If terminal mode is turned on
        if(terminal_mode) {
          // neco
        } else  //  terminal mode is off
        {
          uint8_t input = Serial1.read();
          if(input == CONNECTIONSTATUS) {
            connectionActive = true;
          }
          if(input == SWITCHPOWER) {
            wait_for_start();
          }
          if(input == ADDTOFRONTMOTORS) {
            int l = Rotors.get_front_left() + 5;
            int r = Rotors.get_front_right() + 5;
            Rotors.set_values_front(l, r);
          }
          if(input == STOPALLMOTORS) {
            Rotors.set_values(1000, 1000, 1000, 1000);
          }
          if(input == SLOWALLMOTORS) {
            int fl = Rotors.get_front_left() - 5;
            int fr = Rotors.get_front_right() - 5;
            int rl = Rotors.get_rear_left() - 5;
            int rr = Rotors.get_rear_right() - 5;
            Rotors.set_values(fl, fr, rl, rr);
          }
        }
      }
    }

    void wait_for_start() {
      uint8_t input;
      while (true) {
        Serial1.print("\rStandby Mode - Pres B to start");
        while (Serial1.available() > 0) {
          input = Serial1.read();
          if (input == SWITCHPOWER ) {
            break;
          }
        }
        if (input == SWITCHPOWER ) {
          break;
        }
        digitalWrite(REDLED,HIGH);
        delay(500);
        digitalWrite(REDLED,LOW);
        delay(1500);
      }
    }

    void print_status() {
      Serial1.print("FIL:");             //Filtered angle
      Serial1.print(accel_t_gyro_global.x_angle, 2);
      Serial1.print(",");
      Serial1.print(accel_t_gyro_global.y_angle, 2);
      Serial1.print(",");
      Serial1.print(accel_t_gyro_global.z_angle, 2);
    }
private:
  bool connectionActive;
  long lastConnectionTime;
  bool terminal_mode;
};

#endif
