#ifndef CALLS_STORE_H
#define CALLS_STORE_H

#include "components/pkb/stores/stmt_stmt_store.h"

class CallStore : public StmtStmtStore {
 private:
  std::unordered_map<std::string, std::unordered_set<std::string>> call_stmt_map;

 public:
  explicit CallStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                     std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type);
  void AddCallerHelper(std::string const &caller, std::string const &callee);
  [[nodiscard]] bool IsCallsPairValid(std::pair<std::string, std::string> const &pair);
  [[nodiscard]] bool IsCallsStarPairValid(std::pair<std::string, std::string> const &pair);
  void AddCallStmtMap(std::string proc, std::string stmt);
  [[nodiscard]] std::unordered_set<std::string> GetCallStmtOf(std::string proc);
  [[nodiscard]] std::unordered_set<std::string> GetCallersOf(std::string const &proc);
  [[nodiscard]] std::unordered_set<std::string> GetCalleesOf(std::string const &proc);
  [[nodiscard]] std::unordered_set<std::string> GetCallersStarOf(StmtType type, std::string const &proc);
  [[nodiscard]] std::unordered_set<std::string> GetCalleesStarOf(StmtType type, std::string const &proc);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllCalls();
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllCallsStar();
};

#endif //CALLS_STORE_H
