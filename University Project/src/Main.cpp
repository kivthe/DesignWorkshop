#include "Server.h"
#include "CmdArgManager.h"
#include "ServerSettingsParser.h"
#include "Databases.h"
#include <thread>
#include <chrono>

int main(int argc, char** argv) {
  kiv::CmdArgvManager argv_man(argc, argv);
  auto settings = kiv::ParseSettingsFromCommandLine(argv_man);
  kiv::Server server;
  server.Run(settings);

  kiv::SQLite sqlite;
  sqlite.CreateDatabase("database.db3");

  sqlite.ExecuteLine("CREATE TABLE test_table (id INTEGER PRIMARY KEY, ratio FLOAT, name TEXT)");
  for (int i = 0; i < 10; ++i) {
    sqlite.ExecuteLine("INSERT INTO test_table VALUES (NULL, 3.14, \'test\')");
  }

  /*
  sqlite.SetQuery("SELECT * FROM test_table");
  sqlite.ExecuteQuerySingle();
  auto vec = sqlite.ExecuteQuerySingle();
  for (auto& c : vec) {
    std::cout << c << '\n';
  }
  */
  

  sqlite.SaveDatabase();
  sqlite.UnloadDatabase();

  std::string command_string;
  while (command_string != "quit") {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    std::getline(std::cin, command_string);
  }
  return 0;
}