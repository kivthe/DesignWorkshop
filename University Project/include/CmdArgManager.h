#ifndef CMDARGMANAGER_H
#define CMDARGMANAGER_H

#include <string>
#include <vector>

namespace kiv {

class CmdArgvManager {
public:
  CmdArgvManager(int argc, char** argv);
  std::string& operator[](std::size_t index);

public:
  int GetArgumentCount() const noexcept;
  std::string GetArgument(std::size_t index) const noexcept;

private:
  int argc_;
  std::vector<std::string> argv_;
};

}

#endif