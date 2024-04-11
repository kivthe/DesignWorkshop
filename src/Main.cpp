#include "Server.h"

int main(int argc, char** argv) {
  dw::Server server;
  server.Setup();
  server.Start();

  return 0;
}