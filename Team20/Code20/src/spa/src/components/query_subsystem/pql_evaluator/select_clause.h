#ifndef SELECT_CLAUSE_H_
#define SELECT_CLAUSE_H_

#include "clause.h"
#include "components/pkb/pkb.h"
#include "components/query_subsystem/pql_lexer/pql_token.h"
#include "components/query_subsystem/pql_parser/parsed_query.h"

namespace pql {

class SelectClause : public Clause {
 public:
  SelectClause(const PqlToken &selected_synonym, const std::unordered_map<std::string, DesignEntityType> &declarations, PKB *pkb);
  Table Execute() override;
 private:
  PqlToken selected_synonym;
  std::unordered_map<std::string, DesignEntityType> declarations;
  PKB *pkb;
  DesignEntityType GetSynonymDesignEntity(const PqlToken &arg);
  StmtType GetStmtType(const DesignEntityType &design_entity);

};

}

#endif //SELECT_CLAUSE_H_
