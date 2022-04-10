#ifndef CALLS_CLAUSE_H_
#define CALLS_CLAUSE_H_

#include "clause.h"
#include "components/query_subsystem/pql_lexer/pql_token.h"
#include "components/pkb/pkb.h"

namespace pql {

class CallsClause : public Clause {
 public:
  CallsClause(const PqlToken &first_arg,
              const PqlToken &second_arg,
              const pkb::PkbPtr &pkb);
  Table Execute() override;
  bool ExecuteBool() override;
  std::set<std::string> GetSynonyms() override;
  size_t GetSynonymsSize() override;
  size_t GetWeight() override;
 private:
  size_t weight = 4;
  PqlToken first_arg;
  PqlToken second_arg;
  const pkb::PkbPtr &pkb;
  using handler = Table (CallsClause::*)();
  const std::map<std::pair<PqlTokenType, PqlTokenType>, handler> execute_handler{
      {{PqlTokenType::SYNONYM, PqlTokenType::SYNONYM}, &CallsClause::HandleSynonymSynonym},
      {{PqlTokenType::SYNONYM, PqlTokenType::UNDERSCORE}, &CallsClause::HandleSynonymWildcard},
      {{PqlTokenType::SYNONYM, PqlTokenType::IDENT_WITH_QUOTES}, &CallsClause::HandleSynonymIdent},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::SYNONYM}, &CallsClause::HandleWildcardSynonym},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::UNDERSCORE}, &CallsClause::HandleWildcardWildcard},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::IDENT_WITH_QUOTES}, &CallsClause::HandleWildcardIdent},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::SYNONYM}, &CallsClause::HandleIdentSynonym},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::UNDERSCORE}, &CallsClause::HandleIdentWildcard},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::IDENT_WITH_QUOTES}, &CallsClause::HandleIdentIdent},

  };
  Table HandleSynonymSynonym();
  Table HandleSynonymWildcard();
  Table HandleSynonymIdent();
  Table HandleWildcardSynonym();
  Table HandleWildcardWildcard();
  Table HandleWildcardIdent();
  Table HandleIdentSynonym();
  Table HandleIdentWildcard();
  Table HandleIdentIdent();
  using bool_handler = bool (CallsClause::*)();
  const std::map<std::pair<PqlTokenType, PqlTokenType>, bool_handler> execute_bool_handler{
      {{PqlTokenType::SYNONYM, PqlTokenType::SYNONYM}, &CallsClause::HandleSynonymSynonymBool},
      {{PqlTokenType::SYNONYM, PqlTokenType::UNDERSCORE}, &CallsClause::HandleSynonymWildcardBool},
      {{PqlTokenType::SYNONYM, PqlTokenType::IDENT_WITH_QUOTES}, &CallsClause::HandleSynonymIdentBool},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::SYNONYM}, &CallsClause::HandleWildcardSynonymBool},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::UNDERSCORE}, &CallsClause::HandleWildcardWildcardBool},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::IDENT_WITH_QUOTES}, &CallsClause::HandleWildcardIdentBool},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::SYNONYM}, &CallsClause::HandleIdentSynonymBool},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::UNDERSCORE}, &CallsClause::HandleIdentWildcardBool},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::IDENT_WITH_QUOTES}, &CallsClause::HandleIdentIdentBool},
  };
  bool HandleSynonymSynonymBool();
  bool HandleSynonymWildcardBool();
  bool HandleSynonymIdentBool();
  bool HandleWildcardSynonymBool();
  bool HandleWildcardWildcardBool();
  bool HandleWildcardIdentBool();
  bool HandleIdentSynonymBool();
  bool HandleIdentWildcardBool();
  bool HandleIdentIdentBool();
};

}

#endif //CALLS_CLAUSE_H_
