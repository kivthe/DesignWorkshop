#ifndef DW_LOGGER_H
#define DW_LOGGER_H

#include <memory>
#include <string>

namespace dw {

enum class LogLevel : int {
  TRACE,
  DEBUG,
  INFO,
  WARN,
  ERROR,
  CRITICAL,
  LEVEL_OFF,
  COUNT
};

class Logger {
public:
  class Impl;

public:
  Logger(const std::string& name);
  Logger(const Logger&);
  Logger(Logger&&) noexcept;
  Logger& operator=(const Logger&);
  Logger& operator=(Logger&&) noexcept;
  ~Logger();

public:
  template<typename... T>
  void Log(const std::string& format, T&&... args);

private:
  std::unique_ptr<Impl> impl_;
};

}

#endif