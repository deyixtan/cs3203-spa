#ifndef NEXT_STORE_H
#define NEXT_STORE_H

#include "components/pkb/stores/stmt_stmt_store.h"

class ParentStore;
class FollowsStore;

class NextStore : public StmtStmtStore {
 private:
  std::shared_ptr<ParentStore> m_parent_store;
  std::unordered_map<std::string, std::vector<std::string>> m_proc_stmt_map;
  static const std::string SMALLEST_STMT_NO;
  static const std::string LARGEST_STMT_NO;

 public:
  explicit NextStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                     std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type,
                     std::shared_ptr<ParentStore> parent_store);
  void AddNext(IDENT const &before, IDENT const &next);
  void AddNextStar(IDENT const &before, IDENT const &next);
  void AddFirstStmtProc(std::string const &proc, std::string const &stmt);
  void AddLastStmtProc(std::string const &proc, std::string const &stmt);
  void GetUpperStarOfHelper(std::string const &stmt, std::unordered_set<std::string> &res);
  void GetLowerStarOfHelper(std::string const &stmt, std::unordered_set<std::string> &res);
  [[nodiscard]] bool IsNextPairValid(IDENT_PAIR const &pair);
  [[nodiscard]] bool IsNextStarPairValid(IDENT_PAIR const &pair);
  [[nodiscard]] IDENT_SET GetBeforeOf(StmtType type, IDENT const &stmt);
  [[nodiscard]] IDENT_SET GetNextOf(StmtType type, IDENT const &stmt);
  [[nodiscard]] IDENT_SET GetBeforeStarOf(StmtType type, IDENT const &stmt);
  [[nodiscard]] IDENT_SET GetNextStarOf(StmtType type, IDENT const &stmt);
  [[nodiscard]] IDENT_PAIR_VECTOR GetNextPairs();
  [[nodiscard]] IDENT_PAIR_VECTOR GetNextStarPairs();
  [[nodiscard]] IDENT_PAIR_VECTOR GetAllNextStmt(StmtType type1, StmtType type2);
  [[nodiscard]] IDENT_SET GetNextStarSameStmt(StmtType type);
  [[nodiscard]] IDENT_PAIR_VECTOR GetAllNextStarStmt(StmtType type1, StmtType type2);
  std::string GetFirstStmtOfProc(const std::string &stmt);
  std::string GetLastStmtOfProc(const std::string &stmt);
  void InsertPairResultUpper(int start, int end, std::unordered_set<std::string> &res, std::string const stmt);
  void InsertPairResultLower(int start, int end, std::unordered_set<std::string> &res, std::string const stmt);
  std::string GetEndStmtOfWhileLoop(const std::string &smallest_stmt);
  std::string GetStartStmtOfWhileLoop(std::unordered_set<std::string> &ancestor_set);
  void InsertLowerResultForIf(std::string &last_proc_stmt_no, std::string const &stmt, std::string const &prev_if_stmt, std::string const &prev_end_else_stmt, std::unordered_set<std::string> &res, int nesting_level);
  void InsertUpperResultForIf(std::string &first_proc_stmt_no, std::string const &stmt, std::string const &prev_if_stmt, std::unordered_set<std::string> &res, int nesting_level);
  std::string GetFirstElseStmt(std::string const &if_stmt);
  std::string GetEndElseStmt(std::string const &if_stmt);
  void ClearNextStarCache();
  void ComputeNextStore();
  bool IsNextStoreComputed();
};

#endif //NEXT_STORE_H
