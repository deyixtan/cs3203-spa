#include "pattern_store.h"

PatternStore::PatternStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector) :
    StatementStore(move(stmt_vector)) {}

void PatternStore::AddStmtWithPattern(std::string const &stmt, std::string const &lhs, std::string const &rhs) {
  AddPattern(STMT, stmt, lhs, rhs);
}

void PatternStore::AddWhileWithPattern(std::string const &stmt, std::string const &lhs, std::string const &rhs) {
  AddPattern(WHILE, stmt, lhs, rhs);
}

void PatternStore::AddIfWithPattern(std::string const &stmt, std::string const &lhs, std::string const &rhs) {
  AddPattern(IF, stmt, lhs, rhs);
}

std::unordered_set<std::string> PatternStore::GetStmtWithPattern(std::string const &lhs, std::string rhs) {
  return GetPattern(STMT, lhs, rhs);
}

std::unordered_set<std::string> PatternStore::GetWhileWithPattern(std::string const &expr) {
  return GetPattern(WHILE, expr, "");
}

std::unordered_set<std::string> PatternStore::GetIfWithPattern(std::string const &expr) {
  return GetPattern(IF, expr, "");
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PatternStore::GetStmtWithPatternSynonym(std::string rhs) {
  return GetPatternSynonym(STMT, rhs);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PatternStore::GetWhileWithPatternSynonym(std::string rhs) {
  return GetPatternSynonym(WHILE, rhs);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> PatternStore::GetIfWithPatternSynonym(std::string rhs) {
  return GetPatternSynonym(IF, rhs);
}
