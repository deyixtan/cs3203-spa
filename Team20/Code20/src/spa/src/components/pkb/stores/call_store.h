#ifndef CALLS_STORE_H_
#define CALLS_STORE_H_

#include "store.h"

struct CallNode {
  std::unordered_set<std::string> caller_set;
  std::unordered_set<std::string> callee_set;
  std::unordered_set<std::string> caller_star_set;
  std::unordered_set<std::string> callee_star_set;
};

class CallStore : public Store {
 private:
  std::unordered_map<std::string, CallNode> rs_map;

 public:
  explicit CallStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector);
  void AddCaller(std::string const &caller, std::string const &callee);
  void AddCallerStar(std::string const &caller, std::string const &callee);
  [[nodiscard]] std::unordered_set<std::string> GetCallerOf(std::string const &proc);
  [[nodiscard]] std::unordered_set<std::string> GetCalleeOf(std::string const &proc);
  [[nodiscard]] std::unordered_set<std::string> GetCallerStarOf(std::string const &proc);
  [[nodiscard]] std::unordered_set<std::string> GetCalleeStarOf(std::string const &proc);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetAllCallProcs();
};

#endif //CALLS_STORE_H_
