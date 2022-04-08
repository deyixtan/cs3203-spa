#include "store.h"

Store::Store(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
             std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type)
    : m_stmt_vector(move(stmt_vector)), m_stmt_type(move(stmt_type)) {}

IDENT_PAIR_SET Store::GetAllStmt(StmtType type,
                                 std::vector<StmtType> &supported_types,
                                 std::vector<std::pair<std::string,
                                                       std::string>> const &list,
                                 bool checkPairFirst) {
  return GetAllStmt(type, type, supported_types, list, checkPairFirst);
}

IDENT_PAIR_SET Store::GetAllStmt(StmtType type1,
                                 StmtType type2,
                                 std::vector<StmtType> &supported_types,
                                 std::vector<std::pair<std::string,
                                                       std::string>> const &list,
                                 bool checkPairFirst) {
  IDENT_PAIR_SET result;

  if (std::find(supported_types.begin(), supported_types.end(), type1) == supported_types.end()) {
    return result;
  }

  for (auto const &i : list) {
    for (auto const &j : m_stmt_vector->at(type1)) {
      if (checkPairFirst) {
        if (i.first == j) {
          result.push_back(i);
        }
        continue;
      }
      if (i.second == j) {
        result.push_back(i);
      }
    }
  }
  return result;
}
