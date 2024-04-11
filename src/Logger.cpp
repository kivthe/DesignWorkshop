#include <cstddef>
#include <string>
#include <unordered_map>
#include <thread>
#include <vector>
#include <sstream>
#include <ctime>
#include <fstream>
#include <cassert>
#include <stdexcept>
#include <iostream>

#include "Logger.h"
#include "Allocator.h"

namespace dw {

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

static std::unordered_map<LogLevel, const char*> level_map = {
  {LogLevel::Trace,    "TRACE"   },
  {LogLevel::Debug,    "DEBUG"   },
  {LogLevel::Info,     "INFO"    },
  {LogLevel::Warn,     "WARN"    },
  {LogLevel::Error,    "ERROR"   },
  {LogLevel::Critical, "CRITICAL"}
};

//--------------------------------------------------------------------------------

static PoolAllocator<128> LoggerAllocator(128 * 1024);

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

class Logger::Impl {
public:
  struct LogDetails {
    size_t bytes_written;
    LogLevel level;
    char filename[256];
    void* operator new      (size_t count) {
      return LoggerAllocator.Allocate(count);
    }
    void* operator new[]    (size_t count) {
      return LoggerAllocator.Allocate(count);
    }
    void  operator delete   (void* pointer) {
      LoggerAllocator.Deallocate(pointer, sizeof(LogDetails));
    }
    void  operator delete[] (void* pointer, size_t count) {
      LoggerAllocator.Deallocate(pointer, count);
    }
  };
  struct FlagDetails {
    int padding;
    enum Allignment : int {
      Right,
      Left,
      Middle
    } alignment;
    char flag;
  };
public:
  Impl(const std::string& name);
  ~Impl();
  void* operator new      (size_t count);
  void* operator new[]    (size_t count);
  void  operator delete   (void* pointer);
  void  operator delete[] (void* pointer, size_t count);

public:
  void Log(std::string message);
  void Log(std::string message, LogLevel level);
  void LogToFile(const std::string& filename, std::string message);
  void LogToFile(const std::string& filename, std::string message, LogLevel level);

public:
  static std::string GetTimeString();
  static std::string GetDateString();

public:
  LogLevel level;
  std::string name;
  std::string pattern;
  //std::vector<LogDetails> log_details;
};

//--------------------------------------------------------------------------------

Logger::Impl::Impl(const std::string& name) :
  level{LogLevel::Trace},
  name{name},
  pattern{"[%T - %D][%l](%n) %v"}//,
  //log_details{}
{}

//--------------------------------------------------------------------------------

Logger::Impl::~Impl() {}

//--------------------------------------------------------------------------------

void* Logger::Impl::operator new (size_t count) {
  return LoggerAllocator.Allocate(count);
}

//--------------------------------------------------------------------------------

void* Logger::Impl::operator new[] (size_t count) {
  return LoggerAllocator.Allocate(count);
}

//--------------------------------------------------------------------------------

void  Logger::Impl::operator delete   (void* pointer) {
  LoggerAllocator.Deallocate(pointer, sizeof(Logger::Impl));
}

//--------------------------------------------------------------------------------

void  Logger::Impl::operator delete[] (void* pointer, size_t count) {
  LoggerAllocator.Deallocate(pointer, count);
}

//--------------------------------------------------------------------------------

void Logger::Impl::Log(std::string message) {
  if (pattern.empty()) { std::cout << message << '\n'; return; }
  std::string result = pattern;
  size_t index{};
  index = result.find('%', index);
  while (index != std::string::npos) {
    if (index + 1 == result.size()) break;
    switch (result[index + 1]) {
      case 'v':
        result.replace(index, 2, message);
        break;

      case 'n':
        result.replace(index, 2, name);
        break;

      case 'l':
        result.replace(index, 2, level_map[level]);
        break;

      case 'T':
        result.replace(index, 2, GetTimeString());
        break;

      case 'D':
        result.replace(index, 2, GetDateString());
        break;

      case '%':
        result.replace(index, 2, "%");
        break;
      
    }
    index = result.find('%', index + 1);
  }
  std::cout << result << '\n';
}

//--------------------------------------------------------------------------------

void Logger::Impl::Log(std::string message, LogLevel level) {
  LogLevel temp = this->level;
  this->level = level;
  Log(message);
  this->level = temp;
}

//--------------------------------------------------------------------------------

void Logger::Impl::LogToFile(const std::string& filename, std::string message) {
  std::fstream file;
  file.exceptions(std::ios::failbit | std::ios::badbit);
  file.open(filename, std::ios::app);
  if (pattern.empty()) { file << message << '\n'; file.close(); return; }
  std::string result = pattern;
  size_t index{};
  index = result.find('%', index);
  while (index != std::string::npos) {
    if (index + 1 == result.size()) break;
    switch (result[index + 1]) {
      case 'v':
        result.replace(index, 2, message);
        break;

      case 'n':
        result.replace(index, 2, name);
        break;

      case 'l':
        result.replace(index, 2, level_map[level]);
        break;

      case 'T':
        result.replace(index, 2, GetTimeString());
        break;

      case 'D':
        result.replace(index, 2, GetDateString());
        break;

      case '%':
        result.replace(index, 2, "%");
        break;
      
    }
    index = result.find('%', index + 1);
  }
  file << result << '\n';
  file.close();
}

//--------------------------------------------------------------------------------

void Logger::Impl::LogToFile(const std::string& filename, std::string message, LogLevel level) {
  LogLevel temp = this->level;
  this->level = level;
  LogToFile(filename, message);
  this->level = temp;
}

//--------------------------------------------------------------------------------

std::string Logger::Impl::GetTimeString() {
  std::time_t time = std::time(nullptr);
  std::tm* tm = std::localtime(&time);
  int year = tm->tm_year + 1900;
  int month = tm->tm_mon + 1;
  int day = tm->tm_mday;
  char buffer[32];
  int written = std::snprintf(buffer, 32, "%d.%d.%d", month, day, year);
  std::string result(buffer, buffer + written);
  return result;
}

//--------------------------------------------------------------------------------

std::string Logger::Impl::GetDateString() {
  std::time_t time = std::time(nullptr);
  std::tm* tm = std::localtime(&time);
  int hour = tm->tm_hour;
  int minute = tm->tm_min;
  int sec = tm->tm_sec;
  char buffer[32];
  int written = std::snprintf(buffer, 32, "%d:%d:%d", hour, minute, sec);
  std::string result(buffer, buffer + written);
  return result;
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

Logger::Logger(const std::string& name) :
  impl_{new Impl(name)}
{}

//--------------------------------------------------------------------------------

Logger::~Logger() {
  delete impl_;
}

//--------------------------------------------------------------------------------

Logger::Logger(Logger&& src) noexcept :
  impl_{std::move(src.impl_)}
{
  src.impl_ = nullptr;
}

//--------------------------------------------------------------------------------

Logger& Logger::operator=(Logger&& rhs) noexcept {
  impl_ = std::move(rhs.impl_);
  rhs.impl_ = nullptr;
  return *this;
}

//--------------------------------------------------------------------------------

void Logger::SetLogLevel(LogLevel level) {
  impl_->level = level;
}

//--------------------------------------------------------------------------------

void Logger::SetPattern(const std::string& pattern) {
  impl_->pattern = pattern;
}

//--------------------------------------------------------------------------------

void Logger::Log(std::string message) {
  impl_->Log(message);
}

//--------------------------------------------------------------------------------

void Logger::Log(std::string message, LogLevel level) {
  impl_->Log(message, level);
}

//--------------------------------------------------------------------------------

void Logger::LogToFile(const std::string& filename, std::string message) {
  impl_->LogToFile(filename, message);
}

//--------------------------------------------------------------------------------

void Logger::LogToFile(const std::string& filename, std::string message, LogLevel level) {
  impl_->LogToFile(filename, message, level);
}

//--------------------------------------------------------------------------------

std::string Logger::GetName() const {
  return impl_->name;
}

//--------------------------------------------------------------------------------

}