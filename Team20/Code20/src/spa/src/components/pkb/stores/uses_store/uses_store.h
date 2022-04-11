#ifndef USAGE_STORE_H
#define USAGE_STORE_H

#include "components/pkb/pkb_declarations.h"
#include "components/pkb/stores/stmt_var_store.h"

namespace pkb {

// proxy class
class UsesStore : public StmtVarStore {
 public:
  explicit UsesStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                     std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type);
  [[nodiscard]] IDENT_SET GetVarUsedByStmt(std::string const &stmt);
  [[nodiscard]] IDENT_SET GetStmtUsedByVar(StmtType type, std::string const &var);
  [[nodiscard]] IDENT_SET GetAllStmtUsing();
  [[nodiscard]] IDENT_SET GetAllProcUsing();
  [[nodiscard]] IDENT_PAIR_VECTOR GetAllUsesStmt(StmtType type);
};

}

#endif //USAGE_STORE_H
