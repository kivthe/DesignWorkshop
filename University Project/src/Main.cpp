#include "Server.h"
#include "CmdArgManager.h"
#include "ServerSettingsParser.h"
#include "Databases.h"
#include "Format.h"
#include <thread>
#include <chrono>

int main(int argc, char** argv) {
  if (argc > 1 && std::string("help") == argv[1]) {
    std::cout << "config-file <path> - path to configuration file.\n";
    std::cout << "root-dir <path> - path to root folder of server's file navigator.\n";
    std::cout << "log-out-dir <path> - path to where the log files must be written.\n";
    std::cout << "bloacked-addr-file <path> - path to a file, consisting of list of blocked IPv4 addresses.\n";
    std::cout << "http-port <port_num> - manualy specify HTTP port at which the server must listen.\n"; 
    std::cout << "max-cahce-items <count> - manualy specify maximum size of cache manager, beyond which adding new items will delete the eldest ones.\n";
    std::cout << "logs-on <true|false> - manualy specify whether the programs should produce log data.\n";
    return 0;
  }
  kiv::CmdArgvManager argv_man(argc, argv);
  auto settings = kiv::ParseSettingsFromCommandLine(argv_man);
  kiv::Server server;
  server.Run(settings);

  std::string command_string;
  while (command_string != "quit") {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    std::getline(std::cin, command_string);
  }
  return 0;
}