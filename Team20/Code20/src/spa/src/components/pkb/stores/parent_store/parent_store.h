#ifndef PARENT_STORE_H
#define PARENT_STORE_H

#include "components/pkb/pkb_declarations.h"
#include "components/pkb/pkb_relationship.h"
#include "components/pkb/stores/stmt_stmt_store.h"

namespace pkb {

// A store class that maintains all Parent APIs and relationships
class ParentStore : public StmtStmtStore {
 private:

 public:
  explicit ParentStore(std::shared_ptr<std::vector<std::unordered_set<std::string>>> stmt_vector,
                       std::shared_ptr<std::unordered_map<std::string, StmtType>> stmt_type);
  void AddParent(IDENT const &parent, IDENT const &child);
  void AddParentStar(IDENT const &stmt, IDENT_VECTOR const &visited);
  [[nodiscard]] bool IsParentPairValid(IDENT_PAIR const &pair);
  [[nodiscard]] bool IsAnceDescValid(IDENT_PAIR const &pair);
  [[nodiscard]] IDENT GetParentOf(StmtType type, IDENT const &stmt);
  [[nodiscard]] IDENT_SET GetChildOf(StmtType type, IDENT const &stmt);
  [[nodiscard]] IDENT_SET GetAllAnceOf(StmtType type, IDENT const &stmt);
  [[nodiscard]] IDENT_SET GetAllDescOf(StmtType type, IDENT const &stmt);
  [[nodiscard]] IDENT_PAIR_VECTOR GetAllParentStmt(StmtType type1, StmtType type2);
  [[nodiscard]] IDENT_PAIR_VECTOR GetAllParentStarStmt(StmtType type1, StmtType type2);
};

}

#endif //PARENT_STORE_H
