#ifndef PATTERN_ASSIGN_CLAUSE_H_
#define PATTERN_ASSIGN_CLAUSE_H_

#include <map>

#include "clause.h"
#include "components/query_subsystem/pql_lexer/pql_token.h"
#include "components/pkb/pkb.h"

namespace pql {

class PatternAssignClause : public Clause {
 public:
  PatternAssignClause(const std::string &assign_synonym,
                      const PqlToken &first_arg,
                      const PqlToken &second_arg,
                      const pkb::PkbPtr &pkb);

  Table Execute() override;
  bool ExecuteBool() override;
  std::set<std::string> GetSynonyms() override;
  size_t GetSynonymsSize() override;
  size_t GetWeight() override;
 private:
  size_t weight = 2;
  std::string assign_synonym;
  PqlToken first_arg;
  PqlToken second_arg;
  const pkb::PkbPtr &pkb;
  using handler = Table (PatternAssignClause::*)();
  const std::map<std::pair<PqlTokenType, PqlTokenType>, handler> execute_handler{
      {{PqlTokenType::SYNONYM, PqlTokenType::UNDERSCORE}, &PatternAssignClause::HandleSynonymWildcard},
      {{PqlTokenType::SYNONYM, PqlTokenType::EXPR}, &PatternAssignClause::HandleSynonymExact},
      {{PqlTokenType::SYNONYM, PqlTokenType::IDENT_WITH_QUOTES}, &PatternAssignClause::HandleSynonymExact},
      {{PqlTokenType::SYNONYM, PqlTokenType::SUB_EXPRESSION}, &PatternAssignClause::HandleSynonymPartial},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::UNDERSCORE}, &PatternAssignClause::HandleWildcardWildcard},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::EXPR}, &PatternAssignClause::HandleWildcardExact},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::IDENT_WITH_QUOTES}, &PatternAssignClause::HandleWildcardExact},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::SUB_EXPRESSION}, &PatternAssignClause::HandleWildcardPartial},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::UNDERSCORE}, &PatternAssignClause::HandleIdentWildcard},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::EXPR}, &PatternAssignClause::HandleIdentExact},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::IDENT_WITH_QUOTES}, &PatternAssignClause::HandleIdentExact},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::SUB_EXPRESSION}, &PatternAssignClause::HandleIdentPartial}
  };
  Table HandleSynonymWildcard();
  Table HandleSynonymExact();
  Table HandleSynonymPartial();
  Table HandleWildcardWildcard();
  Table HandleWildcardExact();
  Table HandleWildcardPartial();
  Table HandleIdentWildcard();
  Table HandleIdentExact();
  Table HandleIdentPartial();
  using bool_handler = bool (PatternAssignClause::*)();
  const std::map<std::pair<PqlTokenType, PqlTokenType>, bool_handler> execute_bool_handler{
      {{PqlTokenType::SYNONYM, PqlTokenType::UNDERSCORE}, &PatternAssignClause::HandleSynonymWildcardBool},
      {{PqlTokenType::SYNONYM, PqlTokenType::EXPR}, &PatternAssignClause::HandleSynonymExactBool},
      {{PqlTokenType::SYNONYM, PqlTokenType::IDENT_WITH_QUOTES}, &PatternAssignClause::HandleSynonymExactBool},
      {{PqlTokenType::SYNONYM, PqlTokenType::SUB_EXPRESSION}, &PatternAssignClause::HandleSynonymPartialBool},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::UNDERSCORE}, &PatternAssignClause::HandleWildcardWildcardBool},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::EXPR}, &PatternAssignClause::HandleWildcardExactBool},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::IDENT_WITH_QUOTES}, &PatternAssignClause::HandleWildcardExactBool},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::SUB_EXPRESSION}, &PatternAssignClause::HandleWildcardPartialBool},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::UNDERSCORE}, &PatternAssignClause::HandleIdentWildcardBool},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::EXPR}, &PatternAssignClause::HandleIdentExactBool},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::IDENT_WITH_QUOTES}, &PatternAssignClause::HandleIdentExactBool},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::SUB_EXPRESSION}, &PatternAssignClause::HandleIdentPartialBool}
  };
  bool HandleSynonymWildcardBool();
  bool HandleSynonymExactBool();
  bool HandleSynonymPartialBool();
  bool HandleWildcardWildcardBool();
  bool HandleWildcardExactBool();
  bool HandleWildcardPartialBool();
  bool HandleIdentWildcardBool();
  bool HandleIdentExactBool();
  bool HandleIdentPartialBool();
};

}

#endif //PATTERN_ASSIGN_CLAUSE_H_
