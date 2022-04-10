#ifndef FOLLOWS_PARENT_STORE_H
#define FOLLOWS_PARENT_STORE_H

#include "store.h"

class ParentStore;

class StmtStmtStore : public Store {
 protected:
  NESTED_TUPLE_MAP type_pair_map;
  NESTED_TUPLE_MAP star_type_pair_map;
  std::shared_ptr<ParentStore> m_parent_store;
  IDENT_PAIR_VECTOR all_pairs;
  IDENT_PAIR_VECTOR all_star_pairs;

 public:
  explicit StmtStmtStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                         std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type);
  void AddUpperLower(StoreType store_type, IDENT const &upper, IDENT const &lower);
  void AddUpperLowerStar(StoreType store_type, IDENT const &upper, IDENT const &lower, IDENT_VECTOR const &visited);
  void AddFollows(bool is_star, StmtType type1, IDENT const &upper, StmtType type2, IDENT const &lower);
  void AddParent(bool is_star, IDENT const &upper, IDENT const &lower, IDENT_VECTOR const &visited);
  void AddCalls(bool is_star, IDENT const &upper, IDENT const &lower);
  void AddNext(bool is_star, StmtType type1, IDENT const &upper, StmtType type2, IDENT const &lower);
  void AddAffects(bool is_star, StmtType type1, IDENT const &upper, StmtType type2, IDENT const &lower);
  void PopulatePairMap(StmtType type1, IDENT upper, StmtType type2, IDENT lower, NESTED_TUPLE_MAP *pair_map);
  void ExhaustiveAddAllStmt(StmtType type1, IDENT upper, StmtType type2, IDENT lower, bool is_star);
  void ExhaustiveAddSubStmt(StmtType type1, IDENT upper, StmtType type2, IDENT lower, NESTED_TUPLE_MAP *pair_map);
  [[nodiscard]] bool IsValid(IDENT_PAIR const &pair);
  [[nodiscard]] bool IsStarValid(IDENT_PAIR const &pair);
  [[nodiscard]] std::unordered_set<std::string> GetHelper(StmtType type1, StmtType type2, int index, IDENT const &stmt, bool is_star);
  [[nodiscard]] IDENT GetUpperOf(StmtType stmt_type, IDENT const &stmt);
  [[nodiscard]] IDENT GetLowerOf(StmtType stmt_type, IDENT const &stmt);
  [[nodiscard]] IDENT_SET GetUpperSetOf(StoreType store_type, StmtType stmt_type, IDENT const &stmt);
  [[nodiscard]] IDENT_SET GetLowerSetOf(StoreType store_type, StmtType stmt_type, IDENT const &stmt);
  [[nodiscard]] IDENT_SET GetUpperStarOf(StoreType store_type, StmtType stmt_type, IDENT const &stmt);
  [[nodiscard]] IDENT_SET GetLowerStarOf(StoreType store_type, StmtType stmt_type, IDENT const &stmt);
  [[nodiscard]] IDENT_PAIR_VECTOR GetPairByType(StmtType type1, StmtType type2);
  [[nodiscard]] IDENT_PAIR_VECTOR GetStarPairByType(StmtType type1, StmtType type2);
  [[nodiscard]] IDENT_PAIR_VECTOR GetAllPairs();
  [[nodiscard]] IDENT_PAIR_VECTOR GetAllStarPairs();
};

#endif //FOLLOWS_PARENT_STORE_H