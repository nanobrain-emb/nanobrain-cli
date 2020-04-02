#ifndef PROJECT_HPP
#define PROJECT_HPP

#include <string>
#include "command.hpp"

class Project: public Command{
  private:
    std::string env_lang;
    std::string env_board;
    std::string name;

  public:
    Project(std::string, std::string);
    int overlay();
    int py_deps(int, char **);
    std::string get_board();
    std::string get_lang();
    void set_name(char *);
    void _new(char *);
    int verify_nanobrain();
    int new_py(char *);
    void env();
};

#endif //PROJECT_HPP