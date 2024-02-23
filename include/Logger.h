#ifndef DW_LOGGER_H
#define DW_LOGGER_H

#include <memory>
#include <string>

namespace dw {

enum class LogLevel : int {
  Trace,
  Debug,
  Info,
  Warn,
  Error,
  Critical,
  Count
};

class Logger {
public:
  Logger(const std::string& name);
  Logger(const Logger&);
  Logger(Logger&&) noexcept;
  Logger& operator=(const Logger&);
  Logger& operator=(Logger&&) noexcept;

public:
  void SetLogLevel(LogLevel level);
  void Log(const std::string& message);
  void Log(const std::string& message, LogLevel level);
  template<typename... Args>
  void Log(const std::string& format, Args&&... args);
  template<typename... Args>
  void Log(const std::string& format, LogLevel level, Args&&... args);
  void LogToFile(const std::string& filename, const std::string& message);
  template<typename... Args>
  void LogToFile(const std::string& filename, const std::string& format, Args&&... args);
  void LogToFile(const std::string& filename, const std::string& message, LogLevel level);
  template<typename... Args>
  void LogToFile(const std::string& filename, const std::string& format, LogLevel level, Args&&... args);

private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}

#endif