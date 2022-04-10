#ifndef MODIFY_STORE_H
#define MODIFY_STORE_H

#include "components/pkb/stores/stmt_var_store.h"

namespace pkb {

// proxy class
class ModifiesStore : public StmtVarStore {
 public:
  explicit ModifiesStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                         std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type);
  [[nodiscard]] IDENT_SET GetVarModByStmt(IDENT const &stmt);
  [[nodiscard]] IDENT_SET GetStmtModByVar(StmtType type, IDENT const &var);
  [[nodiscard]] IDENT_SET GetAllStmtModify();
  [[nodiscard]] IDENT_SET GetAllProcModify();
  [[nodiscard]] IDENT_PAIR_VECTOR GetAllModStmt(StmtType type);
};

}

#endif //MODIFY_STORE_H
