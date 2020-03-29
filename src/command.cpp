#include "command.hpp" 

// Commands
std::string Command::exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    result.erase(std::remove (result.begin(), result.end(), '\n'), result.end());
    return result;
}

int Command::exec_cmd(std::string cmd, std::string msg) {
  std::cout << this->color << msg << this->e_color << std::endl;
  system(cmd.c_str());
  return 1;
}

int Command::exec_cmd(std::string cmd) {
  std::cout << this->color << cmd << this->e_color << std::endl;
  exec_cmd(cmd, "");
  return 1;
}