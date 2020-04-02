#include <unistd.h>
#include <stdexcept>

#include "project.hpp"

Project::Project(std::string board, std::string lang) : Command() {
  env_lang = lang;
  env_board = board;
}

void Project::set_name(char * n) {
  this->name = n;
}
std::string Project::get_board(){
  return this->env_board;
}
std::string Project::get_lang(){
  return this->env_lang;
}
void Project::env(){
  msg("BOARD: " + this->env_board);
  msg("LANG: " + this->env_lang);
}
void Project::_new(char * name) {
  if (this->env_lang == "python") new_py(name);
}
// Check codes
int Project::verify_nanobrain() {
  std::string pwd = exec_str("pwd");
  pwd += "/.nanobrain";
  if (access(pwd.c_str(), F_OK) == -1) {
    std::cout << "This folder is not a NanoBrain project." << std::endl;
    return 0;
  }
  return 1;
}
int Project::new_py(char * name){
  std::string pwd = exec_str("pwd")+ "/" + name;
  std::string str_proj = name;

  exec_cmd("git clone https://github.com/nanobrain-emb/python-structure.git " + str_proj);
  exec_cmd("cd " + pwd + " && git remote rename origin upstream && git checkout -b master");
  exec_cmd("mkdir -p " + pwd + "/assets/img", "Make assets img folders .");
  exec_cmd("mkdir -p " + pwd + "/assets/fonts", "Make assets fonts folders ..");
  exec_cmd("mkdir -p " + pwd + "/overlay", "Make overlay folders ..");
  exec_cmd("mkdir -p " + pwd + "/test", "Make test folder!");
  

  std::cout << "\e[38;5;46m****************************************************************";
  std::cout << std::endl << std::endl << std::endl <<  "\e[38;5;93m\tTo Start:\e[0m\e[38;5;46m" << std::endl;
  std::cout << std::endl <<  "\t\t cd " << str_proj << std::endl << std::endl << std::endl;
  std::cout << "****************************************************************\e[0m"<<std::endl;
  return 1;
}

int Project::overlay(){

  std::string pwd = exec_str("pwd");

  if(env_lang == "py") {
    exec_cmd("rm -rf overlay/my_app");
    exec_cmd("rsync -av --progress . overlay/my_app/ --exclude overlay/");
  }
  return 1;
}

// CLI
int Project::py_deps(int argc, char ** argv){
  std::string value, folder;
  std::string pwd = exec_str("pwd");
  for (int i = 1; i < argc; i++) {
    value = argv[i];
    if(value == "install") {
      i++;
      for (int j=i; j< argc; j++) {
        value = argv[j];
        folder = exec_str(("python /usr/local/bin/get_pypi --package " + value + " --folder " + pwd + "/lib/").c_str());
        std::cout << folder.c_str() <<std::endl ;
      }
    }
  }
  return 1;
}