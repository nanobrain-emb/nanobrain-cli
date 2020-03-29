#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <string>

class Command {
  private:
    std::string color = "\e[48;5;177m";
    std::string e_color = "\e[0m";
  public:
    Command();
    std::string exec(const char* );
    int exec_cmd(std::string, std::string);
    int exec_cmd(std::string);
};

#endif // COMMAND_HPP