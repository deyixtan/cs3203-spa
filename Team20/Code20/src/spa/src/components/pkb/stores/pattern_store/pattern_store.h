#ifndef PATTERN_STORE_H
#define PATTERN_STORE_H

#include "components/pkb/stores/store.h"
#include "components/pkb/stores/pattern_store/expression_tree/expression_tree.h"

class PatternStore : Store {
 private:
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> m_stmt_pattern_pairs;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> m_if_pattern_pairs;
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> m_while_pattern_pairs;
  std::unordered_map<std::string, std::pair<std::string, std::string>> m_stmt_pattern_map;
 public:
  explicit PatternStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector);
  void AddStmtWithPattern(std::string const &stmt, std::string const &lhs, std::string const &rhs);
  void AddWhileWithPattern(std::string const &stmt, std::string const &expr);
  void AddIfWithPattern(std::string const &stmt, std::string const &expr);
  [[nodiscard]] std::unordered_set<std::string> GetStmtWithPatternExact(std::string lhs, std::string rhs); // a("x" / _ , "x")
  [[nodiscard]] std::unordered_set<std::string> GetStmtWithPatternPartial(std::string lhs, std::string rhs); // a("x"/ _, _"x"_)
  [[nodiscard]] std::unordered_set<std::string> GetStmtWithPatternWildcard(std::string lhs); // a("x"/ _, _)
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetStmtWithPatternSynonymExact(std::string expr); // a(v, "x")
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetStmtWithPatternSynonymPartial(std::string expr); //a(v, _"x"_)
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetStmtWithPatternSynonymWildcard(); // a(v, _)
  [[nodiscard]] std::unordered_set<std::string> GetIfWithPattern(std::string var);
  [[nodiscard]] std::unordered_set<std::string> GetWhileWithPattern(std::string var);
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetIfWithPatternSynonym();
  [[nodiscard]] std::unordered_set<std::pair<std::string, std::string>, pair_hash> GetWhileWithPatternSynonym();
};

#endif //PATTERN_STORE_H
