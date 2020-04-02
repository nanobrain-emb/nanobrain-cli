#ifndef CONTAINER_CTRL_HPP
#define CONTAINER_CTRL_HPP

#include "command.hpp"
#include "project.hpp"

#define DOCKER    "nanobrain/"

class Container_CTRL : public Project{
  private:
    std::string env_board;
    std::string env_lang;
    std::string docker_repo;
    std::string docker_id;
  public:
    Container_CTRL(Project);
    Container_CTRL(std::string, std::string);
    int verify_img();
    int start();
    int terminal();
    int stop();
    int commit(int, char **);
    void exec(std::string);
    int cp(std::string, std::string);
    int cp(std::string, std::string, std::string);
    int is_start();
};

#endif // CONTAINER_CTRL_HPP