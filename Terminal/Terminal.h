#ifndef _Terminal_h
#define _Terminal_h

#include "../PID/PID_v1.h"

#define ENDTERMINAL 255

class Terminal {
  public:
    Terminal();
    void process_commands();
    void parse_line(String);
    void get_help();
    void manage_pid(String);
    void get_PID_settings();
    void set_PID_settings(String);
};

extern Terminal tty;

#endif
