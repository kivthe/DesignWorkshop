#include "Server.h"
#include "Logger.h"
#include "PageManager.h"
#include "httplib.h"

#include <thread>
#include <fstream>

namespace dw {

//--------------------------------------------------------------------------------

class Server::Impl {
public:
  Impl();
  ~Impl();

public:
  void Setup();
  void Start();
  void Shutdown();

public:
  Logger server_logger;
  httplib::Server* server;
  //PageManager* page_manager;
};

//--------------------------------------------------------------------------------

Server::Impl::Impl() :
  server_logger("Server Logger"),
  server{nullptr}
{}

Server::Impl::~Impl() {
  Shutdown();
}

void Server::Impl::Setup() {
  if (server != nullptr) return;
  server = new httplib::Server();
  server_logger.Log("Server setup");
  server_logger.LogToFile("logs.txt", "Server setup");
}

void Server::Impl::Start() {
  
  server->Get(R"(\/pages\/(\d+))", [](const httplib::Request &req, httplib::Response &res) {
    auto number = req.matches[1];
    //res.set_content(R"()", "text/html");
    std::string source_string;
    if (req.matches[1] == "0")
    {
    std::fstream file("pages/0/index.html");
    auto beg = file.tellg();
    file.seekg(0, std::ios::end);
    auto end = file.tellg();
    file.seekg(0, std::ios::beg);
    size_t size = end - beg;
    source_string.reserve(size);
    file.read((char*)source_string.data(), size);
    file.close();
    }
    res.set_content(source_string, "text/html");
  });

  server->set_error_handler([](const httplib::Request &req, httplib::Response &res) {
    char buffer[512];
    std::snprintf(buffer, sizeof(buffer), "<p>Error Status: <span style='color:red;'>%d</span></p>", res.status);
    res.set_content(buffer, "text/html");
  });
  
  server->set_read_timeout(5);
  server->set_write_timeout(5);
  server->listen("0.0.0.0", 8080);
  server_logger.Log("Server started");
  server_logger.LogToFile("logs.txt", "Server started");
}

void Server::Impl::Shutdown() {
  if (server == nullptr) return;
  delete server;
  server = nullptr;
  server_logger.Log("Server shutdown");
  server_logger.LogToFile("logs.txt", "Server shutdown");
}

//--------------------------------------------------------------------------------

Server::Server() :
  impl_(new Impl)
{}

Server::~Server() {
  delete impl_;
}

Server::Server(Server&& src) noexcept  :
  impl_(std::move(src.impl_))
{
  src.impl_ = nullptr;
}
Server& Server::operator=(Server&& rhs) noexcept {
  impl_ = std::move(rhs.impl_);
  rhs.impl_ = nullptr;
  return *this;
}

void Server::Setup() {
  impl_->Setup();
}

void Server::Start() {
  impl_->Start();
}

void Server::Shutdown() {
  impl_->Shutdown();
}

//--------------------------------------------------------------------------------

}