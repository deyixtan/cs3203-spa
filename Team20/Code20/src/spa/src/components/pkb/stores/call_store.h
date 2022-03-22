#ifndef CALLS_STORE_H_
#define CALLS_STORE_H_

#include "store.h"

struct CallNode {
  std::unordered_set<std::string> callers_set;
  std::unordered_set<std::string> callees_set;
  std::unordered_set<std::string> callers_star_set;
  std::unordered_set<std::string> callees_star_set;
};

class CallStore : public Store {
 private:
  std::unordered_map<std::string, CallNode> rs_map;
  std::unordered_map<std::string, std::unordered_set<std::string>> call_stmt_map;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> all_calls_pairs;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> all_calls_star_pairs;

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

#endif //CALLS_STORE_H_
