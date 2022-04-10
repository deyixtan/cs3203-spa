#ifndef CALLS_STORE_H
#define CALLS_STORE_H

#include "components/pkb/stores/stmt_stmt_store.h"

class CallStore : public StmtStmtStore {
 private:
  IDENT_SET_MAP call_stmt_map;

 public:
  explicit CallStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                     std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type);
  void AddCalls(IDENT const &caller, IDENT const &callee);
  [[nodiscard]] bool IsCallsPairValid(IDENT_PAIR const &pair);
  [[nodiscard]] bool IsCallsStarPairValid(IDENT_PAIR const &pair);
  void AddCallStmtMap(IDENT proc, IDENT stmt);
  [[nodiscard]] IDENT_SET GetCallStmtOf(IDENT proc);
  [[nodiscard]] IDENT_SET GetCallersOf(IDENT const &proc);
  [[nodiscard]] IDENT_SET GetCalleesOf(IDENT const &proc);
  [[nodiscard]] IDENT_SET GetCallersStarOf(IDENT const &proc);
  [[nodiscard]] IDENT_SET GetCalleesStarOf(IDENT const &proc);
};

#endif //CALLS_STORE_H
