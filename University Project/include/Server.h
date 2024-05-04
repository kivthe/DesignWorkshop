#ifndef DW_SERVER_H
#define DW_SERVER_H

#include <string>
#include <iostream>

namespace kiv {

struct ServerSettings {
  std::string config_file = "";
  std::string root_directory = "";
  std::string log_output_directory = "";
  std::string blocked_addresses_file = "";
  std::string requests_templates_file = "";
  bool is_log_output_on = true;
  int max_threads = 4;
  int http_port = 8080;
  int max_cache_items = 100;

  friend std::ostream& operator<<(std::ostream& stream, ServerSettings& cfg) {
    stream << cfg.config_file << '\n' << cfg.root_directory << '\n' << cfg.log_output_directory << '\n'
      << cfg.blocked_addresses_file << '\n' << cfg.requests_templates_file << '\n' << cfg.is_log_output_on << '\n'
      << cfg.max_threads << '\n' << cfg.http_port << '\n' << cfg.max_cache_items << '\n';
    return stream;
  }
};

class Server {
public:
  class Impl;

public:
  Server();
  Server(Server&&) noexcept;
  Server& operator=(Server&&) noexcept;
  ~Server();

public:
  void Run(const ServerSettings& settings);
  void Shutdown();

private:
  Impl* impl_;
};

}

#endif