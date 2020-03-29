#include "project.hpp"

Project::Project(std::string lang, char * name) {
  if(lang == "py") new_py(name);
}

int Project::new_py(char * name){
  Command cmd;
  std::string pwd = cmd.exec("pwd")+ "/" + name;
  std::string str_proj = name;

  cmd.exec_cmd("git clone https://github.com/nanobrain-emb/python-structure.git " + str_proj +" -b python");
  cmd.exec_cmd("cd " + pwd + " && git remote rename origin upstream && git checkout -b master");
  cmd.exec_cmd("mkdir -p " + pwd + "/assets/img", "Make assets img folders .");
  cmd.exec_cmd("mkdir -p " + pwd + "/assets/fonts", "Make assets fonts folders ..");
  cmd.exec_cmd("mkdir -p " + pwd + "/overlay", "Make overlay folders ..");
  cmd.exec_cmd("mkdir -p " + pwd + "/test", "Make test folder!");
  

  std::cout << "\e[38;5;46m****************************************************************";
  std::cout << std::endl << std::endl << std::endl <<  "\e[38;5;93m\tTo Start:\e[0m\e[38;5;46m" << std::endl;
  std::cout << std::endl <<  "\t\t cd " << str_proj << std::endl << std::endl << std::endl;
  std::cout << "****************************************************************\e[0m";
  return 1;
}

int Project::py_overlay(){
  Command cmd;
  std::string pwd = cmd.exec("pwd");

  cmd.exec_cmd("rm -rf overlay/my_app");
  
  cmd.exec_cmd("rsync -av --progress . overlay/my_app/ --exclude overlay/");

  return 1;
}

// CLI
int Project::py_deps(int argc, char ** argv){
  Command cmd;
  std::string value, folder;
  std::string pwd = cmd.exec("pwd");
  for (int i = 1; i < argc; i++) {
    value = argv[i];
    if(value == "install") {
      i++;
      for (int j=i; j< argc; j++) {
        value = argv[j];
        folder = cmd.exec(("python /usr/local/bin/nanobrain-cli_py --package " + value + " --folder " + pwd + "/lib/").c_str());
        std::cout << folder.c_str() <<std::endl ;
      }
    }
  }
  return 1;
}