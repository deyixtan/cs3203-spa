#include "pattern_store.h"

PatternStore::PatternStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector) :
    StatementStore(move(stmt_vector)) {}

void PatternStore::AddStmtWithPattern(std::string const &stmt, std::string const &lhs, std::string const &rhs) {
  AddPattern(stmt, lhs, rhs);
}

std::unordered_set<std::string> PatternStore::GetStmtWithPattern(std::string const &lhs, std::string rhs) {
  return GetPattern(lhs, rhs);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PatternStore::GetStmtWithPatternSynonym(std::string rhs) {
  return GetPatternSynonym(rhs);
}
