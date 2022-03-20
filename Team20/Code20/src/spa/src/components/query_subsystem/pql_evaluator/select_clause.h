#ifndef SELECT_CLAUSE_H_
#define SELECT_CLAUSE_H_

#include "clause.h"
#include "components/pkb/pkb.h"
#include "components/query_subsystem/pql_lexer/pql_token.h"
#include "components/query_subsystem/pql_parser/parsed_query.h"
#include "clause_util.h"

namespace pql {

class SelectClause : public Clause {
 public:
  SelectClause(const PqlToken &result_clause, const std::unordered_map<std::string, DesignEntityType> &declarations, PKB *pkb);
  Table Execute() override;
 private:
  PqlToken result_clause;
  std::unordered_map<std::string, DesignEntityType> declarations;
  PKB *pkb;
  DesignEntityType GetSynonymDesignEntity(const PqlToken &arg);

};

}

#endif //SELECT_CLAUSE_H_
