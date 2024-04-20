#include "CmdArgManager.h"
#include <stdexcept>

namespace kiv {

CmdArgvManager::CmdArgvManager(int argc, char** argv) :
  argc_{argc},
  argv_{}
{
  argv_.reserve(argc_);
  for (int i = 0; i < argc_; ++i) {
    std::string arg(reinterpret_cast<const char*>(argv[i]));
    argv_.push_back(std::move(arg));
  }
}

std::string& CmdArgvManager::operator[](std::size_t index) {
  return argv_.at(index);
}

int CmdArgvManager::GetArgumentCount() const noexcept {
  return argc_;
}

std::string CmdArgvManager::GetArgument(std::size_t index) const noexcept {
  if (argv_.empty() || argv_.size() < index) return {};
  return argv_[index];
}

}