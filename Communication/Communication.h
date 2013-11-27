#ifndef _Communication_h
#define _Communication_h

#include "Energia.h"

#define CONNECTIONSTATUS 42

extern bool connectionActive;

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
    }
    void receive() {
      if (Serial1.available() > 0 ) {
        input = Serial1.read();
        if(input == CONNECTIONSTATUS) {
          connectionActive = true;
        }
      }
    }
};

Communication Bluetooth;

#endif
