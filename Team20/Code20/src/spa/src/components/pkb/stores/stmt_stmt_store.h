#ifndef FOLLOWS_PARENT_STORE_H
#define FOLLOWS_PARENT_STORE_H

#include "store.h"

class StmtStmtStore : public Store {
 private:
  NESTED_MAP type_pair_map;
  NESTED_MAP star_type_pair_map;
  STMT_PAIR_SET all_pairs;
  STMT_PAIR_SET all_star_pairs;

 public:
  explicit StmtStmtStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                         std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type);
  void AddUpperLower(StoreType store_type,
                     STMT const &upper,
                     STMT const &lower);
  void AddUpperLowerStar(StoreType store_type,
                         STMT const &upper,
                         STMT const &lower,
                         STMT_VECTOR const &visited);
  void AddFollows(bool is_star, StmtType type1, STMT const &upper, StmtType type2, STMT const &lower);
  void AddParent(bool is_star,
                 STMT const &upper,
                 STMT const &lower,
                 STMT_VECTOR const &visited);
  void AddCalls(bool is_star, STMT const &upper, STMT const &lower);
  void AddNext(bool is_star, StmtType type1, STMT const &upper, StmtType type2, STMT const &lower);
  void PopulatePairMap(StmtType type1,
                       STMT upper,
                       StmtType type2,
                       STMT lower,
                       NESTED_MAP *pair_map);
  void ExhaustiveAddAllStmt(StmtType type1, std::string upper, StmtType type2, std::string lower, bool is_star);
  void ExhaustiveAddSubStmt(StmtType type1,
                            STMT upper,
                            StmtType type2,
                            STMT lower,
                            NESTED_MAP *pair_map);
  void WipeNextStar();
  [[nodiscard]] bool IsValid(STMT_PAIR const &pair);
  [[nodiscard]] bool IsStarValid(STMT_PAIR const &pair);
  [[nodiscard]] bool IsNextStarPairValid(STMT_PAIR const &pair);
  [[nodiscard]] std::unordered_set<std::string> GetHelper(StmtType type1,
                                                          StmtType type2,
                                                          int index,
                                                          STMT const &stmt,
                                                          bool is_star);
  [[nodiscard]] STMT GetUpperOf(StmtType stmt_type, STMT const &stmt);
  [[nodiscard]] STMT GetLowerOf(StmtType stmt_type, STMT const &stmt);
  [[nodiscard]] STMT_SET GetUpperSetOf(StoreType store_type,
                                       StmtType stmt_type,
                                       STMT const &stmt);
  [[nodiscard]] STMT_SET GetLowerSetOf(StoreType store_type,
                                       StmtType stmt_type,
                                       STMT const &stmt);
  [[nodiscard]] STMT_SET GetUpperStarOf(StoreType store_type,
                                        StmtType stmt_type,
                                        STMT const &stmt);
  [[nodiscard]] STMT_SET GetLowerStarOf(StoreType store_type,
                                        StmtType stmt_type,
                                        STMT const &stmt);
  [[nodiscard]] STMT_PAIR_SET GetPairByType(StmtType type1,
                                            StmtType type2);
  [[nodiscard]] STMT_PAIR_SET GetStarPairByType(StmtType type1,
                                                StmtType type2);
  [[nodiscard]] STMT_PAIR_SET GetAllPairs();
  [[nodiscard]] STMT_PAIR_SET GetAllStarPairs();
  [[nodiscard]] STMT_PAIR_SET GetAllNextStarPairs();
  void GetLowerStarOfHelper(StmtType stmt_type,
                            STMT const &stmt,
                            STMT_SET &res,
                            STMT_SET &visited);
  void GetUpperStarOfHelper(StmtType stmt_type,
                            STMT const &stmt,
                            STMT_SET &res,
                            STMT_SET &visited);
};

#endif //FOLLOWS_PARENT_STORE_H
