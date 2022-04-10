#ifndef CALLST_CLAUSE_H_
#define CALLST_CLAUSE_H_

#include "clause.h"
#include "components/query_subsystem/pql_lexer/pql_token.h"
#include "components/pkb/pkb.h"

namespace pql {

class CallsTClause : public Clause {
 public:
  CallsTClause(const PqlToken &first_arg,
               const PqlToken &second_arg,
               const PkbPtr &pkb);
  Table Execute() override;
  bool ExecuteBool() override;
  std::set<std::string> GetSynonyms() override;
  size_t GetSynonymsSize() override;
  size_t GetWeight() override;
 private:
  size_t weight = 11;
  PqlToken first_arg;
  PqlToken second_arg;
  const PkbPtr &pkb;
  using handler = Table (CallsTClause::*)();
  const std::map<std::pair<PqlTokenType, PqlTokenType>, handler> execute_handler{
      {{PqlTokenType::SYNONYM, PqlTokenType::SYNONYM}, &CallsTClause::HandleSynonymSynonym},
      {{PqlTokenType::SYNONYM, PqlTokenType::UNDERSCORE}, &CallsTClause::HandleSynonymWildcard},
      {{PqlTokenType::SYNONYM, PqlTokenType::IDENT_WITH_QUOTES}, &CallsTClause::HandleSynonymIdent},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::SYNONYM}, &CallsTClause::HandleWildcardSynonym},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::UNDERSCORE}, &CallsTClause::HandleWildcardWildcard},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::IDENT_WITH_QUOTES}, &CallsTClause::HandleWildcardIdent},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::SYNONYM}, &CallsTClause::HandleIdentSynonym},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::UNDERSCORE}, &CallsTClause::HandleIdentWildcard},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::IDENT_WITH_QUOTES}, &CallsTClause::HandleIdentIdent},

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
  using bool_handler = bool (CallsTClause::*)();
  const std::map<std::pair<PqlTokenType, PqlTokenType>, bool_handler> execute_bool_handler{
      {{PqlTokenType::SYNONYM, PqlTokenType::SYNONYM}, &CallsTClause::HandleSynonymSynonymBool},
      {{PqlTokenType::SYNONYM, PqlTokenType::UNDERSCORE}, &CallsTClause::HandleSynonymWildcardBool},
      {{PqlTokenType::SYNONYM, PqlTokenType::IDENT_WITH_QUOTES}, &CallsTClause::HandleSynonymIdentBool},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::SYNONYM}, &CallsTClause::HandleWildcardSynonymBool},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::UNDERSCORE}, &CallsTClause::HandleWildcardWildcardBool},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::IDENT_WITH_QUOTES}, &CallsTClause::HandleWildcardIdentBool},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::SYNONYM}, &CallsTClause::HandleIdentSynonymBool},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::UNDERSCORE}, &CallsTClause::HandleIdentWildcardBool},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::IDENT_WITH_QUOTES}, &CallsTClause::HandleIdentIdentBool},
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

#endif //CALLST_CLAUSE_H_
