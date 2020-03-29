#ifndef PROJECT_HPP
#define PROJECT_HPP

#include "command.hpp"

class Project {
  private:
    int new_py(char *);

  public:
    Project(std::string, char *);
    int py_overlay();
    int py_deps(int, char **);
};

#endif //PROJECT_HPP