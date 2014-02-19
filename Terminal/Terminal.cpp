#include "Energia.h"
#include "Terminal.h"
#include "../PID/PID_v1.h"
#include "../ExternDef.h"
#include <string.h>

Terminal::Terminal() {}

void Terminal::process_commands() {
  char input;
  String line = "";
  while(true) {
    if(Serial1.available() > 0) {
      input = Serial1.read();
      if(input==ENDTERMINAL) {
        break;
      }
      else if(input==10) {
        parse_line(line);
        line = "";
        input = 0;
      }
      else {
        line += input;
      }
    }
  }
}

void Terminal::parse_line(String line) {
  int spacer = line.indexOf(" ");
  String command = line.substring(0,spacer);
  if(command == "pid") {
    if(line.length() > (spacer + 1)) {
      String rest = line.substring(spacer+1);
      manage_pid(rest);
    } else { get_help(); }
  }
  else if(command == "online") {
    int cur_time = (millis() - global_info.start_time) / 1000;
    Serial1.print("Time from start : ");
    Serial1.println(cur_time);
  }
  else {
    get_help();
  }
}

void Terminal::get_help() {
  Serial1.println("available commands :");
  Serial1.println("\t - pid set <p> <i> <d>");
  Serial1.println("\t\t - p,i,d are numbers");
  Serial1.println("\t - pid current");
  Serial1.println("");
}

void Terminal::manage_pid(String opts) {
  int spacer = opts.indexOf(" ");
  String action = opts.substring(0, spacer);
  if(action == "current") {
    get_PID_settings();
  } else if (action == "set") {
    if(opts.length() > (spacer + 1)) {
      String rest = opts.substring(spacer+1);
      set_PID_settings(rest);
    } else { get_help(); }
  }
}

void Terminal::get_PID_settings() {
  Serial1.println("Current PID settings :");
  Serial1.print("\t p : ");
  Serial1.println(x_pilot.GetKp());
  Serial1.print("\t i : ");
  Serial1.println(x_pilot.GetKi());
  Serial1.print("\t d : ");
  Serial1.println(x_pilot.GetKd());
}

void Terminal::set_PID_settings(String values) {
  double p;
  double i;
  double d;
  int first_spacer = values.indexOf(" ");
  int second_spacer = values.indexOf(" ", first_spacer + 1);
  char tmp[10];
  values.substring(0,first_spacer).toCharArray(tmp,10);
  p = atof(tmp);
  values.substring(first_spacer + 1,second_spacer).toCharArray(tmp,10);
  i = atof(tmp);
  values.substring(second_spacer + 1).toCharArray(tmp,10);
  d = atof(tmp);
  x_pilot.SetTunings(p, i, d);
  get_PID_settings();
}