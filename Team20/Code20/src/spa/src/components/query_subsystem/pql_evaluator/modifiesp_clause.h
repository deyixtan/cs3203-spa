#ifndef MODIFIESP_CLAUSE_H_
#define MODIFIESP_CLAUSE_H_

#include <map>

#include "clause.h"
#include "components/pkb/pkb.h"
#include "components/query_subsystem/pql_lexer/pql_token.h"
#include "components/query_subsystem/pql_parser/parsed_query.h"

namespace pql {

class ModifiesPClause : public Clause {
 public:
  ModifiesPClause(const std::unordered_map<std::string, DesignEntityType> &declarations,
                  const PqlToken &first_arg,
                  const PqlToken &second_arg,
                  const pkb::PkbPtr &pkb);
  Table Execute() override;
  bool ExecuteBool() override;
  std::set<std::string> GetSynonyms() override;
  size_t GetSynonymsSize() override;
  size_t GetWeight() override;
 private:
  size_t weight = 5;
  std::unordered_map<std::string, DesignEntityType> declarations;
  PqlToken first_arg;
  PqlToken second_arg;
  const pkb::PkbPtr &pkb;
  using handler = Table (ModifiesPClause::*)();
  const std::map<std::pair<PqlTokenType, PqlTokenType>, handler> execute_handler{
      {{PqlTokenType::SYNONYM, PqlTokenType::SYNONYM}, &ModifiesPClause::HandleSynonymSynonym},
      {{PqlTokenType::SYNONYM, PqlTokenType::UNDERSCORE}, &ModifiesPClause::HandleSynonymWildcard},
      {{PqlTokenType::SYNONYM, PqlTokenType::IDENT_WITH_QUOTES}, &ModifiesPClause::HandleSynonymIdent},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::SYNONYM}, &ModifiesPClause::HandleIdentSynonym},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::UNDERSCORE}, &ModifiesPClause::HandleIdentWildcard},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::IDENT_WITH_QUOTES}, &ModifiesPClause::HandleIdentIdent},

  };
  Table HandleSynonymSynonym();
  Table HandleSynonymWildcard();
  Table HandleSynonymIdent();
  Table HandleIdentSynonym();
  Table HandleIdentWildcard();
  Table HandleIdentIdent();
  using bool_handler = bool (ModifiesPClause::*)();
  const std::map<std::pair<PqlTokenType, PqlTokenType>, bool_handler> execute_bool_handler{
      {{PqlTokenType::SYNONYM, PqlTokenType::SYNONYM}, &ModifiesPClause::HandleSynonymSynonymBool},
      {{PqlTokenType::SYNONYM, PqlTokenType::UNDERSCORE}, &ModifiesPClause::HandleSynonymWildcardBool},
      {{PqlTokenType::SYNONYM, PqlTokenType::IDENT_WITH_QUOTES}, &ModifiesPClause::HandleSynonymIdentBool},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::SYNONYM}, &ModifiesPClause::HandleIdentSynonymBool},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::UNDERSCORE}, &ModifiesPClause::HandleIdentWildcardBool},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::IDENT_WITH_QUOTES}, &ModifiesPClause::HandleIdentIdentBool},
  };
  bool HandleSynonymSynonymBool();
  bool HandleSynonymWildcardBool();
  bool HandleSynonymIdentBool();
  bool HandleIdentSynonymBool();
  bool HandleIdentWildcardBool();
  bool HandleIdentIdentBool();
};

}

#endif //MODIFIESP_CLAUSE_H_
