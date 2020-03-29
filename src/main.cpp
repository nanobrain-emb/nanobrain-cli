#include <unistd.h>
#include <stdexcept>

#include "container_ctrl.hpp"
#include "project.hpp"
#include "firmware.hpp"
#include "command.hpp"

std::string env_board     = "";
std::string env_lang      = "";

int get_env() {
  char * var;
  var = getenv("NANOBRAIN_BOARD");
  if(var != NULL){
    env_board = var;
    var = getenv("NANOBRAIN_LANG");
    if(var != NULL){
      env_lang = var;
      return 1;
    }
  } else {
    return 0;
  }
  return 0;
}

int main(int argc, char ** argv){
  std::string value;
  std::cout << "NanoBrain - CLI" << std::endl;

  if (get_env() != 1) {
    std::cout << std::endl << "Set the environment variables." << std::endl << std::endl;
    std::cout << std::endl << "\tNANOBRAIN_BOARD\t[rpi3]" << std::endl;
    std::cout << "\tNANOBRAIN_LANG\t[python]" << std::endl << std::endl;
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
      }
    } else {
      return (0);
    }
  }
  return 0;
}