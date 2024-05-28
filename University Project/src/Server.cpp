#include "Server.h"

#include "httplib/httplib.h"

#include "Logger.h"
#include "DataCacheManager.h"

#include <unordered_map>
#include <filesystem>

std::unordered_map<std::string, std::string> ContentLookup = 
{
  {"jsx",  "text/javascript"},
  {"html", "text/html"},
  {"css",  "text/css"}
};

namespace kiv {

//================================================================================

class Server::Impl {
public:
  Impl();
  ~Impl();

public:
  void Run(const ServerSettings& settings);
  void Shutdown();

public:
  httplib::Server* server;
  Logger server_logger;
  ServerSettings settings;
  DataCacheManager cache_manager;
};

//================================================================================

Server::Impl::Impl() :
  server{nullptr},
  server_logger("Server Logger"),
  settings{}
{}

//================================================================================

Server::Impl::~Impl() {
  Shutdown();
}

//================================================================================

void Server::Impl::Run(const ServerSettings& settings) {
  if (server != nullptr) return;
  this->settings = settings;
  server = new httplib::Server();
  server_logger.Log("Server setup");
  server_logger.LogToFile("logs.txt", "Server setup");

  server->Get(R"(^\/(pages\/)?(\d+\/)?([a-z]+\.([a-z]+)))", [&](const httplib::Request &req, httplib::Response &res) {
    std::string path = req.matches[1];
    path += req.matches[2];
    path += req.matches[3];
    if (!cache_manager.CachePresentViaPath(path)) {
      if (!cache_manager.AddCacheFromFile(path, path)) {
        res.status = 404; // Not found
        return;
      }
    }
    auto ptr = cache_manager.GetCacheViaPath(path);
    if (ptr == nullptr) {
      res.status = 500; // Internal server error
      return;
    }
    auto size = cache_manager.QueryCacheDataViaPath(path).size;
    std::string content_type;
    if (ContentLookup.count(req.matches[4]) < 1) content_type = "text/plain";
    else content_type = ContentLookup[req.matches[4]];
    res.set_content(std::string(ptr, ptr + size), content_type);
    server_logger.Log(CFormat("Transfered %d bytes to remote address %s", size, req.remote_addr.c_str()));
    server_logger.LogToFile("logs.txt", CFormat("Transfered %d bytes to remote address %s", size, req.remote_addr.c_str()));
  });

  server->Get(R"(^\/(pages\/)?(\d+\/)?)", [&](const httplib::Request &req, httplib::Response &res) {
    std::string path = req.matches[1];
    path += req.matches[2];
    path += "index.html";
    if (!cache_manager.CachePresentViaPath(path)) {
      if (!cache_manager.AddCacheFromFile(path, path)) {
        res.status = 404; // Not found
        return;
      }
    }
    auto ptr = cache_manager.GetCacheViaPath(path);
    if (ptr == nullptr) {
      res.status = 500; // Internal server error
      return;
    }
    auto size = cache_manager.QueryCacheDataViaPath(path).size;
    res.set_content(std::string(ptr, ptr + size), "text/html");
    server_logger.Log(CFormat("Transfered %d bytes to remote address %s", size, req.remote_addr.c_str()));
    server_logger.LogToFile("logs.txt", CFormat("Transfered %d bytes to remote address %s", size, req.remote_addr.c_str()));
  });

  server->set_error_handler([](const httplib::Request &req, httplib::Response &res) {
    char buffer[512];
    std::snprintf(buffer, sizeof(buffer), "<p>Error Status: <span style='color:red;'>%d</span></p>", res.status);
    res.set_content(buffer, "text/html");
  });

  //================================================================================

  server->set_read_timeout(3);
  server->set_write_timeout(3);
  std::thread listen_thread([&](httplib::Server* server, const char* ip, int port) {
    server->listen("0.0.0.0", port);
  }, server, "0.0.0.0", settings.http_port);
  listen_thread.detach();
  server_logger.Log("Server started");
  server_logger.LogToFile("logs.txt", "Server started");
}

//================================================================================

void Server::Impl::Shutdown() {
  if (server == nullptr) return;
  delete server;
  server = nullptr;
  server_logger.Log("Server shutdown");
  server_logger.LogToFile("logs.txt", "Server shutdown");
}

//================================================================================

Server::Server() :
  impl_(new Impl)
{}

//================================================================================

Server::~Server() {
  delete impl_;
}

//================================================================================

Server::Server(Server&& src) noexcept  :
  impl_(std::move(src.impl_))
{
  src.impl_ = nullptr;
}

//================================================================================

Server& Server::operator=(Server&& rhs) noexcept {
  impl_ = std::move(rhs.impl_);
  rhs.impl_ = nullptr;
  return *this;
}

//================================================================================

void Server::Run(const ServerSettings& settings) {
  impl_->Run(settings);
}

//================================================================================

void Server::Shutdown() {
  impl_->Shutdown();
}

//================================================================================

}