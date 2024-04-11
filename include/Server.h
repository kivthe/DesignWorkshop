#ifndef DW_SERVER_H
#define DW_SERVER_H

namespace dw {

class Server {
public:
  class Impl;

public:
  Server();
  Server(Server&&) noexcept;
  Server& operator=(Server&&) noexcept;
  ~Server();

public:
  void Setup();
  void Start();
  void Shutdown();

private:
  Impl* impl_;
};

}

#endif