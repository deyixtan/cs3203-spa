#ifndef PATTERN_WHILE_CLAUSE_H_
#define PATTERN_WHILE_CLAUSE_H_

#include "clause.h"
#include "components/query_subsystem/pql_lexer/pql_token.h"
#include "components/pkb/pkb.h"

namespace pql {

class PatternWhileClause : public Clause {
 public:
  PatternWhileClause(const std::string &while_synonym,
                     const PqlToken &first_arg,
                     const PkbPtr &pkb);

  Table Execute() override;
  bool ExecuteBool() override;
  std::set<std::string> GetSynonyms() override;
  size_t GetSynonymsSize() override;
  size_t GetWeight() override;
 private:
  size_t weight = 2;
  std::string while_synonym;
  PqlToken first_arg;
  const PkbPtr &pkb;
  using handler = Table (PatternWhileClause::*)();
  const std::map<PqlTokenType, handler> execute_handler{
      {PqlTokenType::SYNONYM, &PatternWhileClause::HandleSynonym},
      {PqlTokenType::UNDERSCORE, &PatternWhileClause::HandleWildcard},
      {PqlTokenType::IDENT_WITH_QUOTES, &PatternWhileClause::HandleIdent},
  };
  Table HandleSynonym();
  Table HandleWildcard();
  Table HandleIdent();
  using bool_handler = bool (PatternWhileClause::*)();
  const std::map<PqlTokenType, bool_handler> execute_bool_handler{
      {PqlTokenType::SYNONYM, &PatternWhileClause::HandleSynonymBool},
      {PqlTokenType::UNDERSCORE, &PatternWhileClause::HandleWildcardBool},
      {PqlTokenType::IDENT_WITH_QUOTES, &PatternWhileClause::HandleIdentBool},
  };
  bool HandleSynonymBool();
  bool HandleWildcardBool();
  bool HandleIdentBool();
};

}

#endif //PATTERN_WHILE_CLAUSE_H_
