#ifndef _Communication_h
#define _Communication_h

#include "Energia.h"
#include "../ExternDef.h"
#include "../Motors/Motors.h"

// Communication constants
#define CONNECTIONSTATUS  42
#define SWITCHPOWER       255
#define ADDTOFRONTMOTORS  100
#define STOPALLMOTORS     3
#define SLOWALLMOTORS     5

extern bool connectionActive;
extern long lastConnectionTime;

class Communication {
  public:
    Communication() {
      Serial1.begin(9600);
    };

    void send(char* text) {
      Serial1.println(text);
    };

    void checkConnection() {
      Serial1.print("#CON:");
      Serial1.println("Test for reply");
      if (lastConnectionTime - millis() > 2000) {
        Rotors.setValues(1000, 1000, 1000, 1000);
      }
      lastConnectionTime = millis();
    }

    void receive() {
      if (Serial1.available() > 0 ) {
        uint8_t input = Serial1.read();
        if(input == CONNECTIONSTATUS) {
          connectionActive = true;
        }
        if(input == SWITCHPOWER) {
          wait_for_start();
        }
        if(input == ADDTOFRONTMOTORS) {
          int l = Rotors.getFrontLeft() + 5;
          int r = Rotors.getFrontRight() + 5;
          Rotors.setValuesFront(l, r);
        }
        if(input == STOPALLMOTORS) {
          Rotors.setValues(1000, 1000, 1000, 1000);
        }
        if(input == SLOWALLMOTORS) {
          int fl = Rotors.getFrontLeft() - 5;
          int fr = Rotors.getFrontRight() - 5;
          int rl = Rotors.getRearLeft() - 5;
          int rr = Rotors.getRearRight() - 5;
          Rotors.setValues(fl, fr, rl, rr);
        }
      }
    }

    void wait_for_start() {
      uint8_t input;
      while (true) {
        Serial1.println("Waiting for Start");
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
};

Communication Bluetooth;

#endif
