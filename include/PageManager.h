#ifndef DW_PAGEMANAGER_H
#define DW_PAGEMANAGER_H

#include <string>

namespace dw {

class PageManager {
public:
  PageManager();
  ~PageManager();

public:
  bool LoadFolder(const std::string& folder, const std::string& request_path);

private:

};

}

#endif