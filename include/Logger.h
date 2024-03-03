#ifndef DW_LOGGER_H
#define DW_LOGGER_H

#include <string>
#include <vector>

#include "Format.h"

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

/*
Logger pattern formating rules:
%v - message to log.
%t - thread id.
%n - logger's name.
%l - log level aka TRACE, DEBUG, ERROR, etc.
%T - current system time in HH:MM:SS format.
%D - current system date in MM/DD/YY format.
%% - the '%' sign.
*/

class Logger {
public:
  class Impl;

public:
  Logger(const std::string& name);
  Logger(const Logger&) = delete;
  Logger(Logger&&) noexcept;
  Logger& operator=(const Logger&) = delete;
  Logger& operator=(Logger&&) noexcept;
  virtual ~Logger();

public:
  void SetLogLevel(LogLevel level);
  void SetPattern(const std::string& pattern);
  void Log(std::string message);
  void Log(std::string message, LogLevel level);
  void LogToFile(const std::string& filename, std::string message);
  void LogToFile(const std::string& filename, std::string message, LogLevel level);

  template<typename... Args>
  void Log(std::string message, Args&&... args) {
    Log(Format(message, args...));
  }
  template<typename... Args>
  void Log(std::string message, LogLevel level, Args&&... args) {
    Log(Format(message, args...), level);
  }
  template<typename... Args>
  void LogToFile(const std::string& filename, std::string message, Args&&... args) {
    LogToFile(filename, Format(message, args...));
  }
  template<typename... Args>
  void LogToFile(const std::string& filename, std::string message, LogLevel level, Args&&... args) {
    LogToFile(filename, Format(message, args...), level);
  }

public:
  std::string GetName() const;

private:
  Impl* impl_;
};

}

#endif