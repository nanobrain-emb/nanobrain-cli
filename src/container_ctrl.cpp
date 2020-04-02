#include "container_ctrl.hpp"

Container_CTRL::Container_CTRL(Project pjt) : Project(pjt.get_board(), pjt.get_lang()){
  env_board = pjt.get_board();
  env_lang = pjt.get_lang();
  docker_repo = DOCKER + env_board + "-" + env_lang;
  docker_id = "";
}
Container_CTRL::Container_CTRL(std::string board, std::string lang): Project(board, lang){
  env_board = board;
  env_lang = lang;
  docker_repo = DOCKER + env_board + "-" + env_lang;
  docker_id = "";
}

int Container_CTRL::verify_img(){
  std::string sha = "";
  try {
    sha = exec_str(("docker images -q " + this->docker_repo).c_str());
    if (sha == "") {
      exec_cmd("docker pull " + this->docker_repo);
      this->docker_id = exec_str(("docker run -d -i " + this->docker_repo).c_str());
      exec("make -C /root/nanobrain-br");

      exec_cmd("docker commit " + this->docker_id + " " + this->docker_repo + ":latest");
    }
    return 1;
  }catch(...){
    return -1;
  }
}

int Container_CTRL::start() {
  if(verify_img()){
    if(exec_str(("docker ps --format '{{.ID}}' --filter id=" + this->docker_id).c_str()) == "") {
      std::cout << "Start VM - " + this->docker_repo << std::endl;
      this->docker_id = exec_str(("docker run -d -i " + this->docker_repo).c_str());
      return 1;
    }
  }
  return 0;
}
int Container_CTRL::terminal() {
  if(verify_img()) {
    exec_cmd(("docker run -it " + this->docker_repo).c_str(), "Starting terminal ...");
    return 1;
  }
  return 0;
}
int Container_CTRL::is_start(){
  if(exec_str(("docker ps --format '{{.ID}}' --filter id=" + this->docker_id).c_str()) != "") {
    return 1;
  }
  return 0;
}

int Container_CTRL::stop(){
    if(is_start()) {
      exec_cmd("docker stop " + this->docker_id, "Stoping VM ..."); 
      exec_cmd("docker rm " + this->docker_id, "Finishing ...");
      return 1;
    }
  return 0;
}

void Container_CTRL::exec(std::string command){
  exec_cmd(("docker exec -it " + this->docker_id + " " + command).c_str());
}

int Container_CTRL::cp(std::string host, std::string target) {
  std::string d_cp = "docker cp ";
  if(host.find(":") != std::string::npos) {
    if(target.find(":") != std::string::npos){
      return -1;
    } else {
      d_cp += host + " " + this->docker_id + target;
    }
  } else {
    d_cp += this->docker_id + host + " " + target;
  }
  exec_cmd(d_cp);
  return 1;
}
int Container_CTRL::cp(std::string host, std::string target, std::string msg) {
  std::string d_cp = "docker cp ";
  if(host.find(":") != std::string::npos) {
    if(target.find(":") != std::string::npos){
      return -1;
    } else {
      d_cp += host + " " + this->docker_id + target;
    }
  } else {
    d_cp += this->docker_id + host + " " + target;
  }
  exec_cmd(d_cp, msg);
  return 1;
}

int Container_CTRL::commit(int argc, char ** argv){
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
  exec_str((commit + this->docker_id + " " + this->docker_repo + docker_suf).c_str());

  return 0;
}