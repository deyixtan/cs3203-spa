#ifndef USESP_CLAUSE_H_
#define USESP_CLAUSE_H_

#include <map>

#include "clause.h"
#include "components/pkb/pkb.h"
#include "components/query_subsystem/pql_lexer/pql_token.h"
#include "components/query_subsystem/pql_parser/parsed_query.h"

namespace pql {

class UsesPClause : public Clause {
 public:
  UsesPClause(const std::unordered_map<std::string, DesignEntityType> &declarations,
              const PqlToken &first_arg,
              const PqlToken &second_arg,
              const pkb::PkbPtr &pkb);
  Table Execute() override;
  bool ExecuteBool() override;
  std::set<std::string> GetSynonyms() override;
  size_t GetSynonymsSize() override;
  size_t GetWeight() override;
 private:
  size_t weight = 7;
  std::unordered_map<std::string, DesignEntityType> declarations;
  PqlToken first_arg;
  PqlToken second_arg;
  const pkb::PkbPtr &pkb;
  using handler = Table (UsesPClause::*)();
  const std::map<std::pair<PqlTokenType, PqlTokenType>, handler> execute_handler{
      {{PqlTokenType::SYNONYM, PqlTokenType::SYNONYM}, &UsesPClause::HandleSynonymSynonym},
      {{PqlTokenType::SYNONYM, PqlTokenType::UNDERSCORE}, &UsesPClause::HandleSynonymWildcard},
      {{PqlTokenType::SYNONYM, PqlTokenType::IDENT_WITH_QUOTES}, &UsesPClause::HandleSynonymIdent},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::SYNONYM}, &UsesPClause::HandleIdentSynonym},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::UNDERSCORE}, &UsesPClause::HandleIdentWildcard},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::IDENT_WITH_QUOTES}, &UsesPClause::HandleIdentIdent},

  };
  Table HandleSynonymSynonym();
  Table HandleSynonymWildcard();
  Table HandleSynonymIdent();
  Table HandleIdentSynonym();
  Table HandleIdentWildcard();
  Table HandleIdentIdent();
  using bool_handler = bool (UsesPClause::*)();
  const std::map<std::pair<PqlTokenType, PqlTokenType>, bool_handler> execute_bool_handler{
      {{PqlTokenType::SYNONYM, PqlTokenType::SYNONYM}, &UsesPClause::HandleSynonymSynonymBool},
      {{PqlTokenType::SYNONYM, PqlTokenType::UNDERSCORE}, &UsesPClause::HandleSynonymWildcardBool},
      {{PqlTokenType::SYNONYM, PqlTokenType::IDENT_WITH_QUOTES}, &UsesPClause::HandleSynonymIdentBool},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::SYNONYM}, &UsesPClause::HandleIdentSynonymBool},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::UNDERSCORE}, &UsesPClause::HandleIdentWildcardBool},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::IDENT_WITH_QUOTES}, &UsesPClause::HandleIdentIdentBool},
  };
  bool HandleSynonymSynonymBool();
  bool HandleSynonymWildcardBool();
  bool HandleSynonymIdentBool();
  bool HandleIdentSynonymBool();
  bool HandleIdentWildcardBool();
  bool HandleIdentIdentBool();
};

}

#endif //USESP_CLAUSE_H_
