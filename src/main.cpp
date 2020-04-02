#include <unistd.h>
#include <stdexcept>

#include "container_ctrl.hpp"
#include "project.hpp"
#include "firmware.hpp"
#include "command.hpp"

std::string BOARDS[1] = {"rpi3"};
std::string LANGS[1]  = {"python"};

std::string env_board     = "";
std::string env_lang      = "";

void msg_env() {
  std::cout << std::endl << "Environment variables" << std::endl << std::endl;
  std::cout << std::endl << "\tNANOBRAIN_BOARD\t[rpi3]" << std::endl;
  std::cout << "\tNANOBRAIN_LANG\t[python]" << std::endl << std::endl;
}

int get_env() {
  char * var;
  int flag = 0;
  var = getenv("NANOBRAIN_BOARD");
  if(var != NULL){
    env_board = var;
    var = getenv("NANOBRAIN_LANG");
    if(var != NULL) env_lang = var;
    int size = sizeof BOARDS /sizeof BOARDS[0];
    for (int i=0; i < size; i++) {
      if(env_board.compare(BOARDS[i]) == 0) flag = 1;
    }
    // Verify if is valid
    if(flag) {
      flag = 0;
      size = sizeof LANGS /sizeof LANGS[0];
      for (int i=0; i < size; i++) {
        if(env_lang.compare(LANGS[i]) == 0) flag = 1;
      }
      if(flag) return 1;
    }
    std::cout << std::endl << "ERROR:" << std::endl;
    std::cout << "BOARD=" << env_board << std::endl;
    std::cout << "LANG=" << env_lang << std::endl;
    return 0;
  }
  return 0;
}

int main(int argc, char ** argv){
  std::string value;
  std::cout << "NanoBrain - CLI" << std::endl;

  if (get_env() != 1) {
    std::cout << std::endl << "NOT SET:" << std::endl;
    msg_env();
    return -1;
  }
  Project project = Project(env_board, env_lang);
  Firmware firmware = Firmware(env_board, env_lang);

  for (int i = 1; i < argc; i++) {
    value = argv[i];

    if(value=="commit") return (firmware.commit(argc, argv));
    if(value=="deps") return (project.py_deps(argc, argv));
    
    if(value == "new") {
      i++;
      project._new(argv[i]);
      return(1);
    }
    if (project.verify_nanobrain()) {
      if ((value == "--firmware") || (value == "-f")) {
        firmware.create();
        firmware.stop();
        return(1);
      }else if ((value=="--burn") || (value=="-b")){
        i++;
        firmware.burn(argv[i]);
        return(1);
      }else if(value=="-fb") {
        i++;
        firmware.create();
        firmware.burn(argv[i]);
        firmware.stop();
        return(1);
      }else if(value == "--logo-bootup") {
        i++;
        firmware.logo_bootup(argv[i]);
      } else if(value == "start") {
        firmware.start();
      } else if(value == "stop") {
        firmware.stop();
      } else if(value == "terminal") {
        firmware.terminal();
        return(1);
      } else if(value == "env") {
        project.env();
        return(1);
      }else if((value == "--help") || (value == "-h")){

        return (1);
      }
    } else {
      return (0);
    }
  }
  return 0;
}