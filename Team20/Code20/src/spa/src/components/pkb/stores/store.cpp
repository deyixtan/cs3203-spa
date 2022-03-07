#include "store.h"

Store::Store(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector) : m_stmt_vector(move(stmt_vector)) {}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> Store::GetAllStmt(StmtType type,
                                                                                     std::vector<StmtType> &supported_types,
                                                                                     std::unordered_set<std::pair<std::string,
                                                                                                                  std::string>,
                                                                                                        pair_hash> const &list,
                                                                                     bool checkPairFirst) {
  return GetAllStmt(type, type, supported_types, list, checkPairFirst);
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> Store::GetAllStmt(StmtType type1,
                                                                                     StmtType type2,
                                                                                     std::vector<StmtType> &supported_types,
                                                                                     std::unordered_set<std::pair<std::string,
                                                                                                                  std::string>,
                                                                                                        pair_hash> const &list,
                                                                                     bool checkPairFirst) {
  if (std::find(supported_types.begin(), supported_types.end(), type1) == supported_types.end()) {
    throw std::runtime_error("INVALID STATEMENT TYPE");
  }

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> result;
  for (auto const &i : list) {
    for (auto const &j : m_stmt_vector->at(type1)) {
      if (checkPairFirst) {
        if (i.first == j) {
          result.insert(i);
        }
        continue;
      }
      if (i.second == j) {
        result.insert(i);
      }
    }
  }
  return result;
}
