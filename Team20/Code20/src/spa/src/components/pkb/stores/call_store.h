#ifndef CALLS_STORE_H
#define CALLS_STORE_H

#include "bi_directional_store.h"

class CallStore : public BidirectionalStore {
 private:
  std::unordered_map<std::string, std::unordered_set<std::string>> call_stmt_map;

 public:
  explicit CallStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector);
  void AddCallerHelper(std::string const &caller, std::string const &callee);
  [[nodiscard]] bool IsCallsPairExists(std::string const &first_proc, std::string const &second_proc);
  [[nodiscard]] bool IsCallsStarPairExists(std::string const &first_proc, std::string const &second_proc);
  void AddCallStmtMap(std::string proc, std::string stmt);
  [[nodiscard]] std::unordered_set<std::string> GetCallStmtOf(std::string proc);
  [[nodiscard]] std::unordered_set<std::string> GetCallersOf(std::string const &proc);
  [[nodiscard]] std::unordered_set<std::string> GetCalleesOf(std::string const &proc);
  [[nodiscard]] std::unordered_set<std::string> GetCallersStarOf(std::string const &proc);
  [[nodiscard]] std::unordered_set<std::string> GetCalleesStarOf(std::string const &proc);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllCalls();
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllCallsStar();
};

#endif //CALLS_STORE_H
