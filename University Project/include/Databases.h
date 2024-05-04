#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <memory>
#include <cstdint>
#include <vector>
#include <ostream>
#include <variant>

namespace kiv {

// TODO: Inherrit from base interface
class SQLite {
public:
  class Impl;

  struct Column {
    std::string name = "";
    std::variant<std::int64_t, double, std::string, void*> variant = (void*)0;
    std::int32_t size_bytes = 0;
  };

public:
  SQLite();
  ~SQLite();

public:
  bool CreateDatabase(const std::string& db_filename) noexcept;
  bool LoadDatabase(const std::string& db_filepath) noexcept;
  bool SaveDatabase() noexcept;
  bool UnloadDatabase() noexcept;

  bool ExecuteLine(const std::string& line) noexcept;
  bool SetQuery(const std::string& query_string) noexcept;

  bool BindQueryValue(const std::string& name          /* NULL VALUE */) noexcept;
  bool BindQueryValue(const std::string& name, std::int32_t       value) noexcept;
  bool BindQueryValue(const std::string& name, std::uint32_t      value) noexcept;
  bool BindQueryValue(const std::string& name, std::int64_t       value) noexcept;
  bool BindQueryValue(const std::string& name, double             value) noexcept;
  bool BindQueryValue(const std::string& name, const std::string& value) noexcept;
  bool BindQueryValue(std::int32_t index           /* NULL VALUE*/) noexcept;
  bool BindQueryValue(std::int32_t index, std::int32_t       value) noexcept;
  bool BindQueryValue(std::int32_t index, std::uint32_t      value) noexcept;
  bool BindQueryValue(std::int32_t index, std::int64_t       value) noexcept;
  bool BindQueryValue(std::int32_t index, double             value) noexcept;
  bool BindQueryValue(std::int32_t index, const std::string& value) noexcept;
  bool ClearQueryBindings() noexcept;
  std::vector<Column> ExecuteQuerySingle() noexcept;
  std::vector<std::vector<Column>> ExecuteQuery() noexcept;

public:
  std::int32_t GetQueryBindingIndex(const std::string& name) const noexcept;

private:
  std::unique_ptr<Impl> impl_;
};

// TODO: Implement. Inherrit from base interface
class SQLExpress {};

// TODO: Implement. Inherrit from base interface
class PostgreSQL {};

}

#endif
