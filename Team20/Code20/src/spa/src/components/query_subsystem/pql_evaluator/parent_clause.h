#ifndef PARENT_CLAUSE_H_
#define PARENT_CLAUSE_H_

#include "clause.h"
#include "components/query_subsystem/pql_parser/parsed_query.h"
#include "components/pkb/pkb.h"

namespace pql {

class ParentClause : public Clause {
 public:
  ParentClause(const std::unordered_map<std::string, DesignEntityType> &declarations,
               const PqlToken &first_arg,
               const PqlToken &second_arg,
               const PkbPtr &pkb);
  Table Execute() override;
  bool ExecuteBool() override;
  std::set<std::string> GetSynonyms() override;
  size_t GetSynonymsSize() override;
  size_t GetWeight() override;
 private:
  size_t weight = 9;
  std::unordered_map<std::string, DesignEntityType> declarations;
  PqlToken first_arg;
  PqlToken second_arg;
  const PkbPtr &pkb;
  using handler = Table (ParentClause::*)();
  const std::map<std::pair<PqlTokenType, PqlTokenType>, handler> execute_handler{
      {{PqlTokenType::SYNONYM, PqlTokenType::SYNONYM}, &ParentClause::HandleSynonymSynonym},
      {{PqlTokenType::SYNONYM, PqlTokenType::UNDERSCORE}, &ParentClause::HandleSynonymWildcard},
      {{PqlTokenType::SYNONYM, PqlTokenType::NUMBER}, &ParentClause::HandleSynonymInteger},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::SYNONYM}, &ParentClause::HandleWildcardSynonym},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::UNDERSCORE}, &ParentClause::HandleWildcardWildcard},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::NUMBER}, &ParentClause::HandleWildcardInteger},
      {{PqlTokenType::NUMBER, PqlTokenType::SYNONYM}, &ParentClause::HandleIntegerSynonym},
      {{PqlTokenType::NUMBER, PqlTokenType::UNDERSCORE}, &ParentClause::HandleIntegerWildcard},
      {{PqlTokenType::NUMBER, PqlTokenType::NUMBER}, &ParentClause::HandleIntegerInteger},

  };
  Table HandleSynonymSynonym();
  Table HandleSynonymWildcard();
  Table HandleSynonymInteger();
  Table HandleWildcardSynonym();
  Table HandleWildcardWildcard();
  Table HandleWildcardInteger();
  Table HandleIntegerSynonym();
  Table HandleIntegerWildcard();
  Table HandleIntegerInteger();
  using bool_handler = bool (ParentClause::*)();
  const std::map<std::pair<PqlTokenType, PqlTokenType>, bool_handler> execute_bool_handler{
      {{PqlTokenType::SYNONYM, PqlTokenType::SYNONYM}, &ParentClause::HandleSynonymSynonymBool},
      {{PqlTokenType::SYNONYM, PqlTokenType::UNDERSCORE}, &ParentClause::HandleSynonymWildcardBool},
      {{PqlTokenType::SYNONYM, PqlTokenType::NUMBER}, &ParentClause::HandleSynonymIntegerBool},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::SYNONYM}, &ParentClause::HandleWildcardSynonymBool},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::UNDERSCORE}, &ParentClause::HandleWildcardWildcardBool},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::NUMBER}, &ParentClause::HandleWildcardIntegerBool},
      {{PqlTokenType::NUMBER, PqlTokenType::SYNONYM}, &ParentClause::HandleIntegerSynonymBool},
      {{PqlTokenType::NUMBER, PqlTokenType::UNDERSCORE}, &ParentClause::HandleIntegerWildcardBool},
      {{PqlTokenType::NUMBER, PqlTokenType::NUMBER}, &ParentClause::HandleIntegerIntegerBool},

  };
  bool HandleSynonymSynonymBool();
  bool HandleSynonymWildcardBool();
  bool HandleSynonymIntegerBool();
  bool HandleWildcardSynonymBool();
  bool HandleWildcardWildcardBool();
  bool HandleWildcardIntegerBool();
  bool HandleIntegerSynonymBool();
  bool HandleIntegerWildcardBool();
  bool HandleIntegerIntegerBool();
};

}

#endif //PARENT_CLAUSE_H_
