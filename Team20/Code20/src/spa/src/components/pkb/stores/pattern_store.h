#ifndef PATTERN_STORE_H
#define PATTERN_STORE_H

#include "statement_store.h"

class PatternStore : public StatementStore {
 public:
  explicit PatternStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector);
  void AddStmtWithPattern(std::string const &stmt, std::string const &lhs, std::string const &rhs);
  [[nodiscard]] std::unordered_set<std::string> GetStmtWithPattern(std::string const &lhs, std::string rhs);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetStmtWithPatternSynonym(std::string rhs);
};

#endif //PATTERN_STORE_H
