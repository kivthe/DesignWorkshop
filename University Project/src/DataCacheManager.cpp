#include "DataCacheManager.h"
#include <stdexcept>
#include <fstream>
#include <filesystem>

namespace kiv {

Buffer& DataCacheManager::operator[](const std::string& name) {
  return cache_.at(name).second;
}

bool DataCacheManager::AddCache(std::uint8_t* data, std::uint32_t size, const std::string& name) noexcept {
  if (data == nullptr || size == 0) return false;
  CacheData cache_data{};
  cache_data.time_added = std::chrono::high_resolution_clock::now().time_since_epoch();
  cache_data.size = size;
  try {
    Buffer buff;
    buff.reserve(size);
    std::memcpy(buff.data(), data, size);
    cache_[name] = {cache_data, std::move(buff)};
  } catch (std::exception& except) {
    return false;
  }
  return true;
}

bool DataCacheManager::AddCacheFromFile(const std::string& filepath, const std::string& name) noexcept {
  if (!std::filesystem::exists(filepath)) return false;
  std::ifstream file;
  file.open(filepath, std::ios::binary);
  if (file.fail() || !file.is_open()) return false;
  std::size_t file_size{};
  file.seekg(0, std::ios::end);
  file_size = file.tellg();
  file.seekg(0, std::ios::beg);
  file_size -= file.tellg();
  Buffer buff;
  buff.resize(file_size);
  file.read((char*)buff.data(), file_size);
  file.close();
  CacheData cache_data;
  cache_data.path = filepath;
  cache_data.size = file_size;
  cache_data.time_added = std::chrono::high_resolution_clock::now().time_since_epoch();
  cache_[name] = { cache_data, std::move(buff) };
  return true;
}

bool DataCacheManager::RemoveCache(const std::string& name) noexcept {
  if (cache_.count(name) < 1) return false;
  return cache_.erase(name);
}

std::uint8_t* DataCacheManager::GetCache(const std::string& name) noexcept {
  if (cache_.count(name) < 1) return nullptr;
  return cache_.at(name).second.data();
}

std::uint8_t* DataCacheManager::GetCacheViaPath(const std::string& path) noexcept {
  if (cache_.empty()) return nullptr;
  for (auto& [name, pair] : cache_) {
    auto& [data, buff] = pair;
    if (data.path != path) continue;
    if (buff.empty()) return nullptr;
    return buff.data();
  }
  return nullptr;
}

std::size_t DataCacheManager::CheckForExpiration() noexcept {
  std::size_t expired_count{};
  for (auto& [name, pair] : cache_) {
    auto& [cache_data, cache] = pair;
    kiv::Time now = std::chrono::high_resolution_clock::now().time_since_epoch();
    kiv::Time& argument = cache_data.life_span_overriden ? cache_data.life_span : global_life_span_;
    if ((now - cache_data.last_accessed) >= argument) {
      cache_data.expired = true;
      ++expired_count;
    }
  }
  return expired_count;
}

std::size_t DataCacheManager::DeleteExpired() noexcept {
  if (cache_.empty()) return 0;
  std::size_t deleted_count{};
  for (auto iter = cache_.begin(); iter != cache_.end(); ++iter) {
    if (!iter->second.first.expired) continue;
    cache_.erase(iter);
    ++deleted_count;
    if (cache_.empty() || iter == cache_.end()) break;
  }

  return deleted_count;
}

bool DataCacheManager::CachePresent(const std::string& name) const noexcept {
  if (cache_.empty()) return false;
  return cache_.count(name) > 0;
}

bool DataCacheManager::CachePresentViaPath(const std::string& path) const noexcept {
  if (cache_.empty()) return false;
  for (auto&[name, pair] : cache_) {
    auto&[data, buff] = pair;
    if (data.path != path) continue;
    return true;
  }
  return false;
}

DataCacheManager::CacheData DataCacheManager::QueryCacheData(const std::string& name) const noexcept {
  if (cache_.count(name) < 1) return {};
  return cache_.at(name).first;
}

DataCacheManager::CacheData DataCacheManager::QueryCacheDataViaPath(const std::string& path) const noexcept {
  if (cache_.empty()) return {};
  for (auto& [name, pair] : cache_) {
    auto& [data, buff] = pair;
    if (data.path != path) continue;
    return data;
  }
  return {};
}

}