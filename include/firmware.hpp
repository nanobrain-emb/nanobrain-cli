#ifndef FIRMWARE_HPP
#define FIRMWARE_HPP

#include <string>
#include "container_ctrl.hpp"

class Firmware: public Container_CTRL{
  private:
    std::string env_board, env_lang, linux_cmd;
    int verify();
    void start_container();
  public:
    Firmware(std::string, std::string);
    int create();
    int del();
    int burn(char *);
    int logo_bootup(char *);
};

#endif // FIRMWARE_HPP