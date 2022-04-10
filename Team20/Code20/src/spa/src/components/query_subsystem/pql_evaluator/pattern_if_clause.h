#ifndef PATTERN_IF_CLAUSE_H_
#define PATTERN_IF_CLAUSE_H_

#include "clause.h"
#include "components/query_subsystem/pql_lexer/pql_token.h"
#include "components/pkb/pkb.h"

namespace pql {

class PatternIfClause : public Clause {
 public:
  PatternIfClause(const std::string &if_synonym,
                  const PqlToken &first_arg,
                  const PkbPtr &pkb);
  Table Execute() override;
  bool ExecuteBool() override;
  std::set<std::string> GetSynonyms() override;
  size_t GetSynonymsSize() override;
  size_t GetWeight() override;
 private:
  size_t weight = 2;
  std::string if_synonym;
  PqlToken first_arg;
  const PkbPtr &pkb;
  using handler = Table (PatternIfClause::*)();
  const std::map<PqlTokenType, handler> execute_handler{
      {PqlTokenType::SYNONYM, &PatternIfClause::HandleSynonym},
      {PqlTokenType::UNDERSCORE, &PatternIfClause::HandleWildcard},
      {PqlTokenType::IDENT_WITH_QUOTES, &PatternIfClause::HandleIdent},
  };
  Table HandleSynonym();
  Table HandleWildcard();
  Table HandleIdent();
  using bool_handler = bool (PatternIfClause::*)();
  const std::map<PqlTokenType, bool_handler> execute_bool_handler{
      {PqlTokenType::SYNONYM, &PatternIfClause::HandleSynonymBool},
      {PqlTokenType::UNDERSCORE, &PatternIfClause::HandleWildcardBool},
      {PqlTokenType::IDENT_WITH_QUOTES, &PatternIfClause::HandleIdentBool},
  };
  bool HandleSynonymBool();
  bool HandleWildcardBool();
  bool HandleIdentBool();
};

}

#endif //PATTERN_IF_CLAUSE_H_
