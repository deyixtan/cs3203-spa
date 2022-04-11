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
  SelectClause(const PqlToken &result_clause,
               const std::unordered_map<std::string, DesignEntityType> &declarations,
               const pkb::PkbPtr &pkb);
  SelectClause(const std::unordered_set<std::string> &table_synonyms,
               const PqlToken &result_clause,
               const std::unordered_map<std::string, DesignEntityType> &declarations,
               const pkb::PkbPtr &pkb);
  Table Execute() override;
  bool ExecuteBool() override;
  std::set<std::string> GetSynonyms() override;
  size_t GetSynonymsSize() override;
  size_t GetWeight() override;
 private:
  std::unordered_set<std::string> table_synonyms;
  PqlToken result_clause;
  std::unordered_map<std::string, DesignEntityType> declarations;
  const pkb::PkbPtr &pkb;
};

}

#endif //SELECT_CLAUSE_H_
