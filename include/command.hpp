#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <string>

#define COLOR     "\e[48;5;177m"
#define E_COLOR   "\e[0m"

class Command {
  public:
    Command();
    std::string exec_str(const char* );
    int exec_cmd(std::string, std::string);
    int exec_cmd(std::string);
};

#endif // COMMAND_HPP