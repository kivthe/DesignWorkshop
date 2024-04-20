#ifndef DW_SERVER_H
#define DW_SERVER_H

#include "CmdArgManager.h"
#include <string>

namespace kiv {

class Server {
public:
  class Impl;

public:
  Server();
  Server(Server&&) noexcept;
  Server& operator=(Server&&) noexcept;
  ~Server();

public:
  void ParseCmdArgv(CmdArgvManager* argv_man);
  void Setup();
  void Start();
  void Shutdown();

private:
  Impl* impl_;
};

}

#endif