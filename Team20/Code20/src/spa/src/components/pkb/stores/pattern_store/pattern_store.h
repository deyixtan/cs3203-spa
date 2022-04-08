#ifndef PATTERN_STORE_H
#define PATTERN_STORE_H

#include "components/pkb/stores/store.h"
#include "components/pkb/stores/pattern_store/expression_tree/expression_tree.h"

class PatternStore : Store {
 private:
  IDENT_PAIR_VECTOR m_stmt_pattern_pairs;
  IDENT_PAIR_VECTOR m_if_pattern_pairs;
  IDENT_PAIR_VECTOR m_while_pattern_pairs;
  IDENT_PAIR_MAP m_stmt_pattern_map;
 public:
  explicit PatternStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                        std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type);
  void AddStmtWithPattern(IDENT const &stmt, IDENT const &lhs, IDENT const &rhs);
  void AddWhileWithPattern(IDENT const &stmt, IDENT const &expr);
  void AddIfWithPattern(IDENT const &stmt, IDENT const &expr);
  [[nodiscard]] IDENT_SET GetStmtWithPatternExact(IDENT lhs, IDENT rhs); // a("x" / _ , "x")
  [[nodiscard]] IDENT_SET GetStmtWithPatternPartial(IDENT lhs, IDENT rhs); // a("x"/ _, _"x"_)
  [[nodiscard]] IDENT_SET GetStmtWithPatternWildcard(IDENT lhs); // a("x"/ _, _)
  [[nodiscard]] IDENT_PAIR_VECTOR GetStmtWithPatternSynonymExact(IDENT expr); // a(v, "x")
  [[nodiscard]] IDENT_PAIR_VECTOR GetStmtWithPatternSynonymPartial(IDENT expr); //a(v, _"x"_)
  [[nodiscard]] IDENT_PAIR_VECTOR GetStmtWithPatternSynonymWildcard(); // a(v, _)
  [[nodiscard]] IDENT_SET GetIfWithPattern(IDENT var);
  [[nodiscard]] IDENT_SET GetWhileWithPattern(IDENT var);
  [[nodiscard]] IDENT_PAIR_VECTOR GetIfWithPatternSynonym();
  [[nodiscard]] IDENT_PAIR_VECTOR GetWhileWithPatternSynonym();
};

#endif //PATTERN_STORE_H
