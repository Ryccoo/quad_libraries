#ifndef _Terminal_h
#define _Terminal_h

#include "../PID/PID_v1.h"
#include "../Communication/Communication.h"

class Terminal {
  public:
    Terminal() {
    }
    void welcome_screen() {}
    void get_help() {}
    void end_screen() {}
    void get_PID_settings() {}
    void set_PID_settings() {}
};

Terminal Tty;

#endif
