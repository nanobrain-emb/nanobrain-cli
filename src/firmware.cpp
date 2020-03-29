#include <unistd.h>

#include "firmware.hpp"

Firmware::Firmware(std::string board, std::string lang) : Container_CTRL(board, lang) {
  env_board = board;
  env_lang = board;
  linux_cmd = " ";
}

int Firmware::verify() {
  std::string pwd = exec_str("pwd");
  
  pwd += "/overlay/firmware.img";

  if (access(pwd.c_str(), F_OK) == -1) {
    return 0;
  }
  return 1;
}

int Firmware::del() {
  if (verify()) {
    exec_cmd("rm overlay/firmware.img");
    return 1;
  }
  return 0;
}


int Firmware::logo_bootup(char * logo) {
  std::string str_logo = logo;

  exec_cmd("pngtopnm "+ str_logo +" | ppmquant -fs 223 | pnmtoplainpnm > logo_linux_custom_224.ppm", "Convert image to ascii");
  
  if(is_start()) start();

  exec("rm /root/nanobrain-br/build/linux-custom/drivers/video/logo/logo_custom_clut224.c");
  exec("rm /root/nanobrain-br/build/linux-custom/drivers/video/logo/logo_custom_clut224.o");

  cp("logo_linux_custom_224.ppm", ":/root/nanobrain-br/build/linux-custom/drivers/video/logo/logo_custom_clut224.ppm");

  this->linux_cmd +="linux-rebuild ";

  return 1;
}

int Firmware::create(){
  std::string folder = "";

  if(del()) std::cout << "Old firmware, deleted!" <<std::endl;

  std::cout <<  "Generating Firmware ..." <<  std::endl;
  
  overlay();
  if(is_start()) start();
    
  if(get_lang() == "py") folder = "my_app/";
  
  cp("overlay/" + folder, ":/root/nanobrain-br/overlay/usr/local/sbin/", "Overlay directory");

  if(this->env_lang == "python") {
    exec("python -m compileall /root/nanobrain-br/overlay/usr/local/sbin/my_app/");
    exec("rm -rf /root/nanobrain-br/overlay/usr/local/sbin/my_app/*.py");
    exec("rm -rf /root/nanobrain-br/overlay/usr/local/sbin/my_app/src/*.py");
    exec("rm -rf /root/nanobrain-br/overlay/usr/local/sbin/my_app/lib/*.py");
  }
  
  exec("make -C /root/nanobrain-br"+ this->linux_cmd +" all");

  this->linux_cmd = " "; //Clean exec

  cp(":/root/nanobrain-br/images/sdcard.img", "overlay/firmware.img", "Copy image...");

  return 1;
}

int Firmware::burn(char * disk){
  std::string str_disk(disk);
  std::string str_rdisk= str_disk;
  std::string host = exec_str("uname -s");

  str_rdisk.insert(5,"r");

  if (verify() != 1) create();

  if(access(disk, F_OK) != -1) {
    if(host == "Darwin") {
      std::cout << "Identified system - Darwin"  << std::endl;
      exec_cmd("diskutil unmountDisk " + str_disk, "Umount Disk...");
      exec_cmd("sudo dd bs=1m if=overlay/firmware.img of="+ str_rdisk +" conv=sync", "Burn firmware in progress...");
      exec_cmd("sudo diskutil eject " + str_rdisk, "Eject Disk...");
      std::cout << "Done!" << std::endl;
    } else {
      std::cout  << "Identified system - Linux"  << std::endl;
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