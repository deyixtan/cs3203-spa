#include "store.h"

Store::Store(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector) : m_stmt_vector(stmt_vector) {}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> Store::GetAllStmt(StmtType type,
                                                                                     std::vector<StmtType> &supported_types,
                                                                                     std::unordered_set<std::pair<std::string,
                                                                                                                  std::string>,
                                                                                                        pair_hash> list,
                                                                                     bool checkPairFirst) {
  if (std::find(supported_types.begin(), supported_types.end(), type) == supported_types.end()) {
    throw std::runtime_error("INVALID STATEMENT TYPE");
  }

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> result;
  for (auto i : list) {
    for (auto j : m_stmt_vector->at(type)) {
      if (checkPairFirst && i.first == j) {
        result.insert(i);
      } else if (!checkPairFirst && i.second == j) {
        result.insert(i);
      }
    }
  }
  return result;
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> Store::GetAllStmt(StmtType type1,
                                                                                     StmtType type2,
                                                                                     std::vector<StmtType> &supported_types,
                                                                                     std::unordered_set<std::pair<std::string,
                                                                                                                  std::string>,
                                                                                                        pair_hash> list,
                                                                                     bool checkPairFirst) {
  if (std::find(supported_types.begin(), supported_types.end(), type1) == supported_types.end()) {
    throw std::runtime_error("INVALID STATEMENT TYPE");
  }

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> result;
  for (auto i : list) {
    for (auto j : m_stmt_vector->at(type1)) {
      if (checkPairFirst && i.first == j) {
        result.insert(i);
      } else if (!checkPairFirst && i.second == j) {
        result.insert(i);
      }
    }
  }
  return result;
}
