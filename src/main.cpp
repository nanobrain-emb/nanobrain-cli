#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <unistd.h>

// Global Variables

std::string clr           = "\e[48;5;177m";
std::string e_clr         = "\e[0m";
std::string docker_repo   = "nanobrain/";
std::string env_board     = "";
std::string env_lang      = "";
std::string docker_id     = "";
std::string docker_exec   = " "; //Space important

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

// Commands
std::string exec(const char* cmd) {
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

int exec_cmd(std::string cmd, std::string msg) {
  std::cout << clr << msg << e_clr << std::endl;
  system(cmd.c_str());
  return 1;
}

int exec_cmd(std::string cmd) {
  std::cout << clr << cmd << e_clr << std::endl;
  exec_cmd(cmd, "");
  return 1;
}

// Check codes
int verify_docker_img(){
  std::string sha = "";
  sha = exec(("docker images -q " + docker_repo + env_board).c_str());
  if (sha != "") return 1;
  return 0;
}

int verify_nanobrain() {
  std::string pwd = exec("pwd");
  pwd += "/.nanobrain";
  if (access(pwd.c_str(), F_OK) == -1) {
    std::cout << "This folder is not a NanoBrain project." << std::endl;
    return 0;
  }
  return 1;
}

int verify_firmware() {
  std::string pwd=exec("pwd");
  
  
  pwd += "/overlay/firmware.img";

  if (access(pwd.c_str(), F_OK) == -1) {
    return 0;
  }
  return 1;
}

int remove_firmware() {
  if (verify_firmware()) {
    exec_cmd("rm overlay/firmware.img");
    return 1;
  }
  return 0;
}

// Create Projects
int new_proj_python(char * proj_name){
  std::string pwd = exec("pwd")+ "/" + proj_name;
  std::string str_proj = proj_name;

  exec_cmd("git clone https://github.com/brunosantanaa/nanobrain-structure.git " + str_proj +" -b python");
  exec_cmd("cd " + pwd + " && git remote rename origin upstrem && git checkout -b master");
  exec_cmd("mkdir -p " + pwd + "/assets/img", "Make assets folders .");
  exec_cmd("mkdir -p " + pwd + "/assets/fonts", "Make assets folders ..");
  exec_cmd("mkdir -p " + pwd + "/overlay", "Make overlay folders ..");
  exec_cmd("mkdir -p " + pwd + "/test", "Make test folder!");
  

  std::cout << "\e[38;5;46m****************************************************************";
  std::cout << std::endl << std::endl << std::endl <<  "\e[38;5;93m\tTo Start:\e[0m\e[38;5;46m" << std::endl;
  std::cout << std::endl <<  "\t\t cd " << str_proj << std::endl << std::endl << std::endl;
  std::cout << "****************************************************************\e[0m";
  return 1;
}

int python_prepare(){
  std::string pwd = exec("pwd");

  exec_cmd("rm -rf overlay/my_app");
  
  exec_cmd("rsync -av --progress . overlay/my_app/ --exclude overlay/");

  return 1;
}

// Docker Controler
int docker_start() {
  if(exec(("docker ps --format '{{.ID}}' --filter id=" + docker_id).c_str()) == "") {
    std::cout << "Start VM - " + docker_repo + env_board << std::endl;
    docker_id = exec(("docker run -d -i " + docker_repo + env_board).c_str());
    return 1;
  }
  return 0;
}

int docker_stop(){
    
    if(exec(("docker ps --format '{{.ID}}' --filter id=" + docker_id).c_str()) != "") {
      exec_cmd("docker stop " + docker_id, "Stoping VM ..."); 
      exec_cmd("docker rm " + docker_id, "Finishing ...");
      return 1;
    }

  return 0;
}


// Firmware manager
int firmware(){

  if(remove_firmware()) std::cout << "Old firmware, deleted!" <<std::endl;

  std::cout << clr << "Generating Firmware ..." << e_clr << std::endl;
  
  docker_start();
  
  if(env_lang == "python") python_prepare();
  
  if (docker_id.length() < 65) {
    
    exec_cmd("docker cp overlay/my_app/ " + docker_id + ":/root/nanobrain-br/overlay/usr/local/sbin/", "Overlay directory");

    if(env_lang == "python") {
      exec_cmd("docker exec -it " + docker_id + " python -m compileall /root/nanobrain-br/overlay/usr/local/sbin/my_app/");
      exec_cmd("docker exec -it " + docker_id + " rm -rf /root/nanobrain-br/overlay/usr/local/sbin/my_app/*.py");
      exec_cmd("docker exec -it " + docker_id + " rm -rf /root/nanobrain-br/overlay/usr/local/sbin/my_app/src/*.py");
      exec_cmd("docker exec -it " + docker_id + " rm -rf /root/nanobrain-br/overlay/usr/local/sbin/my_app/lib/*.py");
    }
    
    exec_cmd("docker exec -ti " + docker_id + " make -C /root/nanobrain-br"+ docker_exec+" all", "Prepare to make");

    docker_exec = " "; //Clean exec

    exec_cmd("docker cp " + docker_id + ":/root/nanobrain-br/images/sdcard.img overlay/firmware.img", "Copying image ...");

  }
  return 1;
}

int burn(char * disk){
  std::string str_disk(disk);
  std::string str_rdisk= str_disk;
  std::string host = exec("uname -s");

  str_rdisk.insert(5,"r");

  if (verify_firmware() != 1) firmware();

  if(access(disk, F_OK) != -1) {
    if(host == "Darwin") {
      std::cout << clr << "Identified system - Darwin" << e_clr << std::endl;
      exec_cmd("diskutil unmountDisk " + str_disk, "Umount Disk...");
      exec_cmd("sudo dd bs=1m if=overlay/firmware.img of="+ str_rdisk +" conv=sync", "Burn firmware in progress...");
      exec_cmd("sudo diskutil eject " + str_rdisk, "Eject Disk...");
      std::cout << "Done!" << std::endl;
    } else {
      std::cout << clr << "Identified system - Linux" << e_clr << std::endl;
      exec_cmd("umount " + str_disk, "Umount Disk...");
      exec_cmd("sudo dd bs=4M if=overlay/firmware.img of="+ str_disk +" conv=fsync", "Burn firmware in progress...");
      exec_cmd("sudo eject " + str_disk, "Eject Disk...");
      std::cout << "Done!" << std::endl;
    }
  } else {
    std::cout << "Disk not found." << std::endl;
  }
  
  return 1;
}


int logo_bootup(char * logo) {
  std::string str_logo = logo;

  exec_cmd("pngtopnm "+ str_logo +" | ppmquant -fs 223 | pnmtoplainpnm > logo_linux_custom_224.ppm", "Convert image to ascii");
  
  docker_start();

  exec_cmd("docker exec -ti "+ docker_id +" rm /root/nanobrain-br/build/linux-custom/drivers/video/logo/logo_custom_clut224.c");
  exec_cmd("docker exec -ti "+ docker_id +" rm /root/nanobrain-br/build/linux-custom/drivers/video/logo/logo_custom_clut224.o");

  exec_cmd("docker cp  logo_linux_custom_224.ppm "+ docker_id +":/root/nanobrain-br/build/linux-custom/drivers/video/logo/logo_custom_clut224.ppm");

  docker_exec +="linux-rebuild ";

  return 1;
}

// CLI
int python_lib(int argc, char ** argv){
  std::string value, folder;
  std::string pwd = exec("pwd");
  if(verify_nanobrain()){
    for (int i = 1; i < argc; i++) {
      value = argv[i];
      if(value == "install") {
        i++;
        for (int j=i; j< argc; j++) {
          value = argv[j];
          folder = exec(("python /usr/local/bin/nanobrain-cli_py --package " + value + " --folder " + pwd + "/lib/").c_str());
          std::cout << folder.c_str() <<std::endl ;
        }
      }
    }
  }
  return 1;
}
int commit(int argc, char ** argv){
  std::string value, tag;
  std::string msg         = "";
  std::string auth        = "";
  std::string commit      = "docker commit ";
  std::string docker_suf  = "";

  for (int i = 1; i < argc; i++) {
    value = argv[i];
    if((value == "-m") || (value == "--message")) {
      i++;
      msg = argv[i];
      commit += "-m '" + msg + "' ";
    } else if ((value == "-a") || (value == "--author")){
      i++;
      auth = argv[i];
      commit += "-a '" + auth + "' ";
    } else if ((value == "-b") || (value == "--branch")){
      i++;
      tag = argv[i];
      docker_suf += ":" + tag;
    } else if ((value == "-h") || (value == "--help")){
      std::cout << std::endl << "Usage: nanobrain-cli commit [OPTIONS]" << std::endl;
      
      std::cout << std::endl << "Create a new image from a container's changes" << std::endl;

      std::cout << std::endl << "Options" << std::endl;
      std::cout << "\t-a, --author string\tAuthor (e.g., 'John Hannibal Smith <hannibal@a-team.com>')" << std::endl;
      std::cout << "\t-b, --branch \t TAG value (e.g. v1.0.0)";
      std::cout << "\t-m, --message string\t Commit message" << std::endl;
    }
  }
  std::cout << (commit + docker_id + docker_repo + env_lang + env_lang + docker_suf).c_str() << std::endl;

  return 0;
}

int main(int argc, char ** argv){
  std::string value;
  std::cout << "NanoBrain - CLI" << std::endl;

  if (get_env() != 1) {
    std::cout << std::endl << "Set the environment variables." << std::endl << std::endl;
    std::cout << std::endl << "\tNANOBRAIN_BOARD\t[rpi3]" << std::endl;
    std::cout << "\tNANOBRAIN_LANG\t[python | elixir]" << std::endl << std::endl;
    return -1;
  }
  for (int i = 1; i < argc; i++) {
    value = argv[i];

    if(value=="commit") return (commit(argc, argv));
    if(value=="py") return (python_lib(argc, argv));
    
    if(value == "-npy") {
      i++;
      new_proj_python(argv[i]);
      return(1);
    }
    if (verify_nanobrain()) {
      if ((value == "--firmware") || (value == "-f")) {
        firmware();
        docker_stop();
        return(1);
      }else if ((value=="--burn") || (value=="-b")){
        i++;
        burn(argv[i]);
        return(1);
      }else if(value=="-fb") {
        i++;
        firmware();
        docker_stop();
        burn(argv[i]);
        return(1);
      }else if(value == "--logo-bootup") {
        i++;
        logo_bootup(argv[i]);
      }
    } else {
      return (0);
    }
      value = argv[i];
  }
  
  docker_stop();
  return 0;
}