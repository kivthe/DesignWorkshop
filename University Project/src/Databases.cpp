#include "Databases.h"

#include "SQLiteCpp/SQLiteCpp.h"

#include "Logger.h"

namespace kiv {

//================================================================================

class SQLite::Impl {
public:
  Impl();
  ~Impl();

public:
  bool CreateDatabase(const std::string& db_filename) noexcept;
  bool LoadDatabase(const std::string& db_filepath) noexcept;
  bool SaveDatabase() noexcept;
  bool UnloadDatabase() noexcept;

  bool ExecuteLine(const std::string& line) noexcept;
  bool SetStatement(const std::string& statement_string) noexcept;

  bool BindStatementValue(const std::string& name          /* NULL VALUE */) noexcept;
  bool BindStatementValue(const std::string& name, std::int32_t       value) noexcept;
  bool BindStatementValue(const std::string& name, std::uint32_t      value) noexcept;
  bool BindStatementValue(const std::string& name, std::int64_t       value) noexcept;
  bool BindStatementValue(const std::string& name, double             value) noexcept;
  bool BindStatementValue(const std::string& name, const std::string& value) noexcept;
  bool BindStatementValue(std::int32_t index           /* NULL VALUE*/) noexcept;
  bool BindStatementValue(std::int32_t index, std::int32_t       value) noexcept;
  bool BindStatementValue(std::int32_t index, std::uint32_t      value) noexcept;
  bool BindStatementValue(std::int32_t index, std::int64_t       value) noexcept;
  bool BindStatementValue(std::int32_t index, double             value) noexcept;
  bool BindStatementValue(std::int32_t index, const std::string& value) noexcept;
  bool ClearStatementBindings() noexcept;

  std::vector<Column> ExecuteQuerySingle() noexcept;
  std::vector<std::vector<Column>> ExecuteQuery() noexcept;

public:
  std::int32_t GetBindingIndex(const std::string& name) const noexcept;

public:
  Logger logger;
  std::unique_ptr<::SQLite::Database> database;
  std::unique_ptr<::SQLite::Statement> statement;
  //std::unique_ptr<::SQLite::Transaction> transaction;
};

//================================================================================

SQLite::Impl::Impl() :
  logger{"SQLite3 Logger"},
  database{nullptr},
  statement{nullptr}//,
  //transaction{nullptr}
{}

//================================================================================

SQLite::Impl::~Impl() {

}

//================================================================================

bool SQLite::Impl::CreateDatabase(const std::string& db_filename) noexcept {
  auto fallback_ptr = database.release();
  try {
    database = std::make_unique<::SQLite::Database>(
      db_filename,
      ::SQLite::OPEN_CREATE | ::SQLite::OPEN_READWRITE);
      return true;
  } catch (std::exception& except) {
    database.reset(fallback_ptr);
    logger.Log(except.what(), LogLevel::Error);
  }
  return false;
}

//================================================================================

bool SQLite::Impl::LoadDatabase(const std::string& db_filepath) noexcept {
  auto fallback_ptr = database.release();
  try {
    database = std::make_unique<::SQLite::Database>(
      db_filepath,
      ::SQLite::OPEN_READWRITE);
    return true;
  }
  catch (std::exception& except) {
    database.reset(fallback_ptr);
    logger.Log(except.what(), LogLevel::Error);
    logger.LogToFile("logs.txt", except.what(), LogLevel::Error);
  }
  return false;
}

//================================================================================

bool SQLite::Impl::SaveDatabase() noexcept {
  if (database == nullptr) return false;
  try {
    database->backup(database->getFilename().c_str(), ::SQLite::Database::Save);
  } catch (std::exception& except) {
    logger.Log(except.what(), LogLevel::Error);
    logger.LogToFile("logs.txt", except.what(), LogLevel::Error);
  }
  return false;
}

//================================================================================

bool SQLite::Impl::UnloadDatabase() noexcept {
  if (database == nullptr) return false;
  statement.reset();
  statement = nullptr;
  database.reset();
  database = nullptr;
  return true;
}

//================================================================================

bool SQLite::Impl::SetStatement(const std::string& statement_string) noexcept {
  if (database == nullptr) return false;
  auto fallback_ptr = statement.release();
  try {
    statement = std::make_unique<::SQLite::Statement>(*database, statement_string);
    return true;
  } catch (std::exception& except) {
    statement.reset(fallback_ptr);
    logger.Log(except.what(), LogLevel::Error);
    logger.LogToFile("logs.txt", except.what(), LogLevel::Error);
  }
  return false;
}


//================================================================================

bool SQLite::Impl::BindStatementValue(const std::string& name) noexcept {
  if (statement == nullptr) return false;
  statement->bind(name);
  return true;
}

//================================================================================

bool SQLite::Impl::BindStatementValue(const std::string& name, std::int32_t value) noexcept {
  if (statement == nullptr) return false;
  try {
    statement->bind(name, value);
    return true;
  } catch (std::exception& except) {
    logger.Log(except.what(), LogLevel::Error);
    logger.LogToFile("logs.txt", except.what(), LogLevel::Error);
  }
  return false;
}

//================================================================================

bool SQLite::Impl::BindStatementValue(const std::string& name, std::uint32_t value) noexcept {
  if (statement == nullptr) return false;
  try {
    statement->bind(name, value);
    return true;
  }
  catch (std::exception& except) {
    logger.Log(except.what(), LogLevel::Error);
    logger.LogToFile("logs.txt", except.what(), LogLevel::Error);
  }
  return false;
}

//================================================================================

bool SQLite::Impl::BindStatementValue(const std::string& name, std::int64_t value) noexcept {
  if (statement == nullptr) return false;
  try {
    statement->bind(name, value);
    return true;
  }
  catch (std::exception& except) {
    logger.Log(except.what(), LogLevel::Error);
    logger.LogToFile("logs.txt", except.what(), LogLevel::Error);
  }
  return false;
}

//================================================================================

bool SQLite::Impl::BindStatementValue(const std::string& name, double value) noexcept {
  if (statement == nullptr) return false;
  try {
    statement->bind(name, value);
    return true;
  }
  catch (std::exception& except) {
    logger.Log(except.what(), LogLevel::Error);
    logger.LogToFile("logs.txt", except.what(), LogLevel::Error);
  }
  return false;
}

//================================================================================

bool SQLite::Impl::BindStatementValue(const std::string& name, const std::string& value) noexcept {
  if (statement == nullptr) return false;
  try {
    statement->bind(name, value);
    return true;
  }
  catch (std::exception& except) {
    logger.Log(except.what(), LogLevel::Error);
    logger.LogToFile("logs.txt", except.what(), LogLevel::Error);
  }
  return false;
}

//================================================================================

bool SQLite::Impl::BindStatementValue(std::int32_t index) noexcept {
  if (statement == nullptr) return false;
  statement->bind(index);
  return true;
}

//================================================================================

bool SQLite::Impl::BindStatementValue(std::int32_t index, std::int32_t value) noexcept {
  if (statement == nullptr) return false;
  try {
    statement->bind(index, value);
    return true;
  }
  catch (std::exception& except) {
    logger.Log(except.what(), LogLevel::Error);
    logger.LogToFile("logs.txt", except.what(), LogLevel::Error);
  }
  return false;
}

//================================================================================

bool SQLite::Impl::BindStatementValue(std::int32_t index, std::uint32_t value) noexcept {
  if (statement == nullptr) return false;
  try {
    statement->bind(index, value);
    return true;
  }
  catch (std::exception& except) {
    logger.Log(except.what(), LogLevel::Error);
    logger.LogToFile("logs.txt", except.what(), LogLevel::Error);
  }
  return false;
}

//================================================================================

bool SQLite::Impl::BindStatementValue(std::int32_t index, std::int64_t value) noexcept {
  if (statement == nullptr) return false;
  try {
    statement->bind(index, value);
    return true;
  }
  catch (std::exception& except) {
    logger.Log(except.what(), LogLevel::Error);
    logger.LogToFile("logs.txt", except.what(), LogLevel::Error);
  }
  return false;
}

//================================================================================

bool SQLite::Impl::BindStatementValue(std::int32_t index, double value) noexcept {
  if (statement == nullptr) return false;
  try {
    statement->bind(index, value);
    return true;
  }
  catch (std::exception& except) {
    logger.Log(except.what(), LogLevel::Error);
    logger.LogToFile("logs.txt", except.what(), LogLevel::Error);
  }
  return false;
}

//================================================================================

bool SQLite::Impl::BindStatementValue(std::int32_t index, const std::string& value) noexcept {
  if (statement == nullptr) return false;
  try {
    statement->bind(index, value);
    return true;
  }
  catch (std::exception& except) {
    logger.Log(except.what(), LogLevel::Error);
    logger.LogToFile("logs.txt", except.what(), LogLevel::Error);
  }
  return false;
}

//================================================================================

bool SQLite::Impl::ClearStatementBindings() noexcept {
  if (statement == nullptr) return false;
  statement->clearBindings();
  return true;
}

//================================================================================

std::int32_t SQLite::Impl::GetBindingIndex(const std::string& name) const noexcept {
  if (statement == nullptr) return -1;
  return statement->getIndex(name.c_str());
}

//================================================================================

std::vector<SQLite::Column> SQLite::Impl::ExecuteQuerySingle() noexcept {
  if (database == nullptr || statement == nullptr) return {};
  try {
    statement->executeStep();
    std::vector<Column> cols;
    cols.reserve(statement->getColumnCount());
    for (int i = 0 ; i < statement->getColumnCount(); ++i) {
      Column col;
      auto scol = statement->getColumn(i);
      //statement->getColumnDeclaredType();
    }
    return cols;
  } catch (std::exception& except) {
    logger.Log(except.what(), LogLevel::Error);
    logger.LogToFile("logs.txt", except.what(), LogLevel::Error);
  }
  return {};
}

//================================================================================

std::vector<std::vector<SQLite::Column>> SQLite::Impl::ExecuteQuery() noexcept {

}

//================================================================================

bool SQLite::Impl::ExecuteLine(const std::string& line) noexcept {
  if (database == nullptr) return false;
  try {
    database->exec(line);
    return true;
  } catch(std::exception& except) {
    logger.Log(except.what(), LogLevel::Error);
    logger.LogToFile("logs.txt", except.what(), LogLevel::Error);
  }
  return false;
}

//================================================================================

SQLite::SQLite() :
  impl_(std::make_unique<Impl>())
{

}

//================================================================================

SQLite::~SQLite() {
  impl_.reset();
}

//================================================================================

bool SQLite::LoadDatabase(const std::string& db_filepath) noexcept {
  return impl_->LoadDatabase(db_filepath);
}

//================================================================================

bool SQLite::CreateDatabase(const std::string& db_filename) noexcept {
  return impl_->CreateDatabase(db_filename);
}

//================================================================================

bool SQLite::SaveDatabase() noexcept {
  return impl_->SaveDatabase();
}

//================================================================================

bool SQLite::UnloadDatabase() noexcept {
  return impl_->UnloadDatabase();
}

//================================================================================

bool SQLite::SetQuery(const std::string& query_string) noexcept {
  return impl_->SetStatement(query_string);
}

//================================================================================

bool SQLite::BindQueryValue(const std::string& name) noexcept {
  return impl_->BindStatementValue(name);
}

//================================================================================

bool SQLite::BindQueryValue(const std::string& name, std::int32_t value) noexcept {
  return impl_->BindStatementValue(name, value);
}

//================================================================================

bool SQLite::BindQueryValue(const std::string& name, std::uint32_t value) noexcept {
  return impl_->BindStatementValue(name, value);
}

//================================================================================

bool SQLite::BindQueryValue(const std::string& name, std::int64_t value) noexcept {
  return impl_->BindStatementValue(name, value);
}

//================================================================================

bool SQLite::BindQueryValue(const std::string& name, double value) noexcept {
  return impl_->BindStatementValue(name, value);
}

//================================================================================

bool SQLite::BindQueryValue(const std::string& name, const std::string& value) noexcept {
  return impl_->BindStatementValue(name, value);
}

//================================================================================

bool SQLite::BindQueryValue(std::int32_t index) noexcept {
  return impl_->BindStatementValue(index);
}

//================================================================================

bool SQLite::BindQueryValue(std::int32_t index, std::int32_t value) noexcept {
  return impl_->BindStatementValue(index, value);
}

//================================================================================

bool SQLite::BindQueryValue(std::int32_t index, std::uint32_t value) noexcept {
  return impl_->BindStatementValue(index, value);
}

//================================================================================

bool SQLite::BindQueryValue(std::int32_t index, std::int64_t value) noexcept {
  return impl_->BindStatementValue(index, value);
}

//================================================================================

bool SQLite::BindQueryValue(std::int32_t index, double value) noexcept {
  return impl_->BindStatementValue(index, value);
}

//================================================================================

bool SQLite::BindQueryValue(std::int32_t index, const std::string& value) noexcept {
  return impl_->BindStatementValue(index, value);
}

//================================================================================

bool SQLite::ClearQueryBindings() noexcept {
  return impl_->ClearStatementBindings();
}

//================================================================================

std::int32_t SQLite::GetQueryBindingIndex(const std::string& name) const noexcept {
  return impl_->GetBindingIndex(name);
}

//================================================================================

std::vector<SQLite::Column> SQLite::ExecuteQuerySingle() noexcept {
  return impl_->ExecuteQuerySingle();
}

//================================================================================

std::vector<std::vector<SQLite::Column>> SQLite::ExecuteQuery() noexcept {
  return impl_->ExecuteQuery();
}

//================================================================================

bool SQLite::ExecuteLine(const std::string& line) noexcept {
  return impl_->ExecuteLine(line);
}

//================================================================================

}