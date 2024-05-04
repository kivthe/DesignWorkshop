#ifndef DATACAHCEMANAGER_H
#define DATACACHEMANAGER_H

#include <vector>
#include <cstdint>
#include <unordered_map>
#include "Clock.h"

namespace kiv {

using Buffer = std::vector<std::uint8_t>;

class DataCacheManager {
public:
  struct CacheData {
    std::string   path = "";
    std::string   url = "";
    std::uint32_t size = 0;
    kiv::Time     last_accessed = {}; // Server system time
    kiv::Time     time_added = {};    // Server system time
    kiv::Time     life_span = {};
    bool          expired = false;
    bool          life_span_overriden = false;
  };

public:
  DataCacheManager() = default;
  ~DataCacheManager() = default;
  Buffer& operator[](const std::string& name);
  
public:
  bool AddCache(std::uint8_t* data, std::uint32_t size, const std::string& name) noexcept;
  bool AddCacheFromFile(const std::string& filepath, const std::string& name) noexcept;
  bool RemoveCache(const std::string& name) noexcept;
  std::uint8_t* GetCache(const std::string& name) noexcept;
  std::uint8_t* GetCacheViaPath(const std::string& path) noexcept;
  std::size_t CheckForExpiration() noexcept;
  std::size_t DeleteExpired() noexcept;

public:
  bool CachePresent(const std::string& name) const noexcept;
  bool CachePresentViaPath(const std::string& path) const noexcept;
  CacheData QueryCacheData(const std::string& name) const noexcept;
  CacheData QueryCacheDataViaPath(const std::string& path) const noexcept;

private:
  std::unordered_map<std::string, std::pair<CacheData, Buffer>> cache_;
  kiv::Time global_life_span_ = kiv::Time::FromSeconds(120);
};

}

#endif