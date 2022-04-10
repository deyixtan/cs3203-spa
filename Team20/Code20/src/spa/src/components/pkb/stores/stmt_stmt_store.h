#ifndef STMT_STMT_STORE_H
#define STMT_STMT_STORE_H

#include "store.h"

class ParentStore;

class StmtStmtStore : public Store {
 protected:
  NESTED_STMT_STMT_MAP type_pair_map;
  NESTED_STMT_STMT_MAP star_type_pair_map;
  std::shared_ptr<ParentStore> m_parent_store;

 public:
  explicit StmtStmtStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                         std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type);
  void PopulatePairMap(StmtType type1, IDENT upper, StmtType type2, IDENT lower, NESTED_STMT_STMT_MAP *pair_map);
  void ExhaustiveAddAllStmt(StmtType type1, IDENT upper, StmtType type2, IDENT lower, bool is_star);
  void ExhaustiveAddSubStmt(StmtType type1, IDENT upper, StmtType type2, IDENT lower, NESTED_STMT_STMT_MAP *pair_map);
  [[nodiscard]] bool IsValid(IDENT_PAIR const &pair);
  [[nodiscard]] bool IsStarValid(IDENT_PAIR const &pair);
  [[nodiscard]] IDENT GetUpperOf(StmtType stmt_type, IDENT const &stmt);
  [[nodiscard]] IDENT GetLowerOf(StmtType stmt_type, IDENT const &stmt);
  [[nodiscard]] IDENT_SET GetHelper(StmtType type1, StmtType type2, int index, IDENT const &stmt, bool is_star);
  [[nodiscard]] IDENT_SET GetUpperSetOf(StoreType store_type, StmtType stmt_type, IDENT const &stmt);
  [[nodiscard]] IDENT_SET GetLowerSetOf(StmtType stmt_type, IDENT const &stmt);
  [[nodiscard]] IDENT_SET GetUpperStarOf(StmtType stmt_type, IDENT const &stmt);
  [[nodiscard]] IDENT_SET GetLowerStarOf(StmtType stmt_type, std::string const &stmt);
  [[nodiscard]] IDENT_PAIR_VECTOR GetPairByType(StmtType type1, StmtType type2);
  [[nodiscard]] IDENT_PAIR_VECTOR GetStarPairByType(StmtType type1, StmtType type2);
};

#endif //STMT_STMT_STORE_H