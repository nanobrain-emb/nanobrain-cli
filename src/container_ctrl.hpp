#ifndef CONTAINER_CTRL_HPP
#define CONTAINER_CTRL_HPP

#include "command.hpp"

class Container_CTRL {
  private:
    Command cmd;
    std::string env_board;
    std::string env_lang;
    std::string docker_repo   = "nanobrain/";
    std::string docker_id;
  public:
    Container_CTRL(std::string, std::string);
    int verify_img();
    int start();
    int stop();
    int commit(int, char **);
    void exec(std::string);
    int cp(std::string, std::string);
    int cp(std::string, std::string, std::string);
};

#endif // CONTAINER_CTRL_HPP