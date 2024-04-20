#include "Server.h"
#include "CmdArgManager.h"
#include <iostream>

int main(int argc, char** argv) {
  kiv::CmdArgvManager argv_man(argc, argv);
  for (int i = 0; i < argv_man.GetArgumentCount(); ++i) {
    std::cout << argv_man[i] << '\n';
  }
  kiv::Server server;
  server.Setup();
  server.Start();

  return 0;
}