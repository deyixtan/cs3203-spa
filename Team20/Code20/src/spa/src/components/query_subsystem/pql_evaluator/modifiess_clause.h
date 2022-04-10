#ifndef MODIFIESS_CLAUSE_H_
#define MODIFIESS_CLAUSE_H_

#include "clause.h"
#include "components/query_subsystem/pql_lexer/pql_token.h"
#include "components/query_subsystem/pql_parser/parsed_query.h"
#include "components/pkb/stores/store.h"
#include "components/pkb/pkb.h"

namespace pql {

class ModifiesSClause : public Clause {
 public:
  ModifiesSClause(const std::unordered_map<std::string, DesignEntityType> &declarations,
                  const PqlToken &first_arg,
                  const PqlToken &second_arg,
                  const PkbPtr &pkb);
  Table Execute() override;
  bool ExecuteBool() override;
  std::set<std::string> GetSynonyms() override;
  size_t GetSynonymsSize() override;
  size_t GetWeight() override;
 private:
  size_t weight = 6;
  std::unordered_map<std::string, DesignEntityType> declarations;
  PqlToken first_arg;
  PqlToken second_arg;
  const PkbPtr &pkb;
  using handler = Table (ModifiesSClause::*)();
  const std::map<std::pair<PqlTokenType, PqlTokenType>, handler> execute_handler{
      {{PqlTokenType::SYNONYM, PqlTokenType::SYNONYM}, &ModifiesSClause::HandleSynonymSynonym},
      {{PqlTokenType::SYNONYM, PqlTokenType::UNDERSCORE}, &ModifiesSClause::HandleSynonymWildcard},
      {{PqlTokenType::SYNONYM, PqlTokenType::IDENT_WITH_QUOTES}, &ModifiesSClause::HandleSynonymIdent},
      {{PqlTokenType::NUMBER, PqlTokenType::SYNONYM}, &ModifiesSClause::HandleIntegerSynonym},
      {{PqlTokenType::NUMBER, PqlTokenType::UNDERSCORE}, &ModifiesSClause::HandleIntegerWildcard},
      {{PqlTokenType::NUMBER, PqlTokenType::IDENT_WITH_QUOTES}, &ModifiesSClause::HandleIntegerIdent},

  };
  Table HandleSynonymSynonym();
  Table HandleSynonymWildcard();
  Table HandleSynonymIdent();
  Table HandleIntegerSynonym();
  Table HandleIntegerWildcard();
  Table HandleIntegerIdent();
  using bool_handler = bool (ModifiesSClause::*)();
  const std::map<std::pair<PqlTokenType, PqlTokenType>, bool_handler> execute_bool_handler{
      {{PqlTokenType::SYNONYM, PqlTokenType::SYNONYM}, &ModifiesSClause::HandleSynonymSynonymBool},
      {{PqlTokenType::SYNONYM, PqlTokenType::UNDERSCORE}, &ModifiesSClause::HandleSynonymWildcardBool},
      {{PqlTokenType::SYNONYM, PqlTokenType::IDENT_WITH_QUOTES}, &ModifiesSClause::HandleSynonymIdentBool},
      {{PqlTokenType::NUMBER, PqlTokenType::SYNONYM}, &ModifiesSClause::HandleIntegerSynonymBool},
      {{PqlTokenType::NUMBER, PqlTokenType::UNDERSCORE}, &ModifiesSClause::HandleIntegerWildcardBool},
      {{PqlTokenType::NUMBER, PqlTokenType::IDENT_WITH_QUOTES}, &ModifiesSClause::HandleIntegerIdentBool},

  };
  bool HandleSynonymSynonymBool();
  bool HandleSynonymWildcardBool();
  bool HandleSynonymIdentBool();
  bool HandleIntegerSynonymBool();
  bool HandleIntegerWildcardBool();
  bool HandleIntegerIdentBool();
};

}

#endif //MODIFIESS_CLAUSE_H_
