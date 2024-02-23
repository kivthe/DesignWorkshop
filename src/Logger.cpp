#include <cstddef>
#include <string>
#include <unordered_map>

#include "spdlog/logger.h"
#include "spdlog/formatter.h"
#include "spdlog/pattern_formatter.h"

#include "Logger.h"
#include "Allocator.h"

namespace dw {

//--------------------------------------------------------------------------------

static PoolAllocator<64> LoggerImplAllocator(64 * 1024);

static const char* LevelToString(LogLevel level) {
  static std::unordered_map<LogLevel, const char*> level_map = {
    {LogLevel::Trace,    "Trace"},
    {LogLevel::Debug,    "Debug"},
    {LogLevel::Info,     "Info"},
    {LogLevel::Warn,     "Warn"},
    {LogLevel::Error,    "Error"},
    {LogLevel::Critical, "Critical"},
  };
  return level_map[level];
}

//--------------------------------------------------------------------------------

class Logger::Impl {
public:
  Impl(const std::string& logger_name) :
    name{logger_name},
    level{LogLevel::Info},
    logger{logger_name}
  {
    logger.set_pattern("[%-20T][%-20n][%-20+](%-20t) %v");
  }
  void* operator new(size_t count) {
    return LoggerImplAllocator.Allocate(count);
  }
  void operator delete(void* pointer) {
    LoggerImplAllocator.Deallocate(pointer, sizeof(Impl));
  }

public:
  std::string name;
  LogLevel level;
  spdlog::logger logger;
};

//--------------------------------------------------------------------------------

Logger::Logger(const std::string& name) :
  impl_{std::make_unique<Impl>(name)}
{}

//--------------------------------------------------------------------------------

void Logger::SetLogLevel(LogLevel level) {
  impl_->level = level;
}

//--------------------------------------------------------------------------------

void Logger::Log(const std::string& message) {
  
}

//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------


}