#include "ServerSettingsParser.h"

#include <fstream>
#include <vector>
#include <unordered_map>
#include <array>
#include <regex>
#include <utility>

namespace kiv {

enum class ParseType : int {
  STRING,
  INTEGRAL,
  BOOLEAN
};

std::unordered_map<std::string, std::pair<ParseType, uintptr_t>> kCommandLineArguments = {
  {"config-file", {ParseType::STRING,       (uintptr_t)&(((ServerSettings*)nullptr)->config_file)}},
  {"root-dir", {ParseType::STRING,          (uintptr_t)&(((ServerSettings*)nullptr)->root_directory)}},
  {"log-out-dir", {ParseType::STRING,       (uintptr_t)&(((ServerSettings*)nullptr)->log_output_directory)}},
  {"blocked-addr-file", {ParseType::STRING, (uintptr_t)&(((ServerSettings*)nullptr)->blocked_addresses_file)}},
  {"templates-file", {ParseType::STRING,    (uintptr_t)&(((ServerSettings*)nullptr)->requests_templates_file)}},
  {"max-threads", {ParseType::INTEGRAL,     (uintptr_t)&(((ServerSettings*)nullptr)->max_threads)}},
  {"http-port", {ParseType::INTEGRAL,       (uintptr_t)&(((ServerSettings*)nullptr)->http_port)}},
  {"max-cache-items", {ParseType::INTEGRAL, (uintptr_t)&(((ServerSettings*)nullptr)->max_cache_items)}},
  {"logs-on", {ParseType::BOOLEAN,          (uintptr_t)&(((ServerSettings*)nullptr)->is_log_output_on)}},
};

ServerSettings ParseSettingsFromCommandLine(CmdArgvManager& manager) {
  int argc = manager.GetArgumentCount();
  std::regex regex;
  regex = R"(--([\w\-\.]+)=([\w\d\.\/\\-]*))";
  ServerSettings config{};
  for (int i = 1; i < argc; ++i) {
    std::smatch match;
    std::string some;
    if (!std::regex_match(manager[i], match, regex)) continue;
    auto iterator = kCommandLineArguments.find(match[1].str());
    if (iterator == kCommandLineArguments.end()) continue;
    auto&[type, ptr] = kCommandLineArguments[iterator->first];
    switch (type) {
    case ParseType::BOOLEAN:
      if (match[2].str().empty()) break;
      if (match[2] == "true") {
        *((bool*)((char*)&config + ptr)) = true;
      }
      if (match[2] == "false") {
        *((bool*)((char*)&config + ptr)) = false;
      }
      break;
    case ParseType::STRING:
      *((std::string*)((char*)&config + ptr)) = match[2].str();
      break;

    case ParseType::INTEGRAL:
      if (match[2].str().empty()) break;
      int val = std::stoi(match[2]);
      int* dest = (int*)((char*)&config + ptr);
      *dest = val;
      break;
    }
  }
  return config;
}

}