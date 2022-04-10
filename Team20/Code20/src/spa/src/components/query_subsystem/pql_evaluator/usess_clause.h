#ifndef USESS_CLAUSE_H_
#define USESS_CLAUSE_H_

#include "clause.h"
#include "components/query_subsystem/pql_lexer/pql_token.h"
#include "components/query_subsystem/pql_parser/parsed_query.h"
#include "components/pkb/stores/store.h"
#include "components/pkb/pkb.h"

namespace pql {

class UsesSClause : public Clause {
 public:
  UsesSClause(const std::unordered_map<std::string, DesignEntityType> &declarations,
              const PqlToken &first_arg,
              const PqlToken &second_arg,
              const PkbPtr &pkb);
  Table Execute() override;
  bool ExecuteBool() override;
  std::set<std::string> GetSynonyms() override;
  size_t GetSynonymsSize() override;
  size_t GetWeight() override;
 private:
  size_t weight = 8;
  std::unordered_map<std::string, DesignEntityType> declarations;
  PqlToken first_arg;
  PqlToken second_arg;
  const PkbPtr &pkb;
  using handler = Table (UsesSClause::*)();
  const std::map<std::pair<PqlTokenType, PqlTokenType>, handler> execute_handler{
      {{PqlTokenType::SYNONYM, PqlTokenType::SYNONYM}, &UsesSClause::HandleSynonymSynonym},
      {{PqlTokenType::SYNONYM, PqlTokenType::UNDERSCORE}, &UsesSClause::HandleSynonymWildcard},
      {{PqlTokenType::SYNONYM, PqlTokenType::IDENT_WITH_QUOTES}, &UsesSClause::HandleSynonymIdent},
      {{PqlTokenType::NUMBER, PqlTokenType::SYNONYM}, &UsesSClause::HandleIntegerSynonym},
      {{PqlTokenType::NUMBER, PqlTokenType::UNDERSCORE}, &UsesSClause::HandleIntegerWildcard},
      {{PqlTokenType::NUMBER, PqlTokenType::IDENT_WITH_QUOTES}, &UsesSClause::HandleIntegerIdent},

  };
  Table HandleSynonymSynonym();
  Table HandleSynonymWildcard();
  Table HandleSynonymIdent();
  Table HandleIntegerSynonym();
  Table HandleIntegerWildcard();
  Table HandleIntegerIdent();
  using bool_handler = bool (UsesSClause::*)();
  const std::map<std::pair<PqlTokenType, PqlTokenType>, bool_handler> execute_bool_handler{
      {{PqlTokenType::SYNONYM, PqlTokenType::SYNONYM}, &UsesSClause::HandleSynonymSynonymBool},
      {{PqlTokenType::SYNONYM, PqlTokenType::UNDERSCORE}, &UsesSClause::HandleSynonymWildcardBool},
      {{PqlTokenType::SYNONYM, PqlTokenType::IDENT_WITH_QUOTES}, &UsesSClause::HandleSynonymIdentBool},
      {{PqlTokenType::NUMBER, PqlTokenType::SYNONYM}, &UsesSClause::HandleIntegerSynonymBool},
      {{PqlTokenType::NUMBER, PqlTokenType::UNDERSCORE}, &UsesSClause::HandleIntegerWildcardBool},
      {{PqlTokenType::NUMBER, PqlTokenType::IDENT_WITH_QUOTES}, &UsesSClause::HandleIntegerIdentBool},

  };
  bool HandleSynonymSynonymBool();
  bool HandleSynonymWildcardBool();
  bool HandleSynonymIdentBool();
  bool HandleIntegerSynonymBool();
  bool HandleIntegerWildcardBool();
  bool HandleIntegerIdentBool();
};

}

#endif //USESS_CLAUSE_H_
