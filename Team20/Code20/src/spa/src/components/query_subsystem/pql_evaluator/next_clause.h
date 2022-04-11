#ifndef NEXT_CLAUSE_H_
#define NEXT_CLAUSE_H_

#include <map>

#include "clause.h"
#include "components/query_subsystem/utils.h"
#include "components/pkb/pkb.h"

namespace pql {

class NextClause : public Clause {
 public:
  NextClause(const std::unordered_map<std::string, DesignEntityType> &declarations,
             const PqlToken &first_arg,
             const PqlToken &second_arg,
             const pkb::PkbPtr &pkb);
  Table Execute() override;
  bool ExecuteBool() override;
  std::set<std::string> GetSynonyms() override;
  size_t GetSynonymsSize() override;
  size_t GetWeight() override;
 private:
  size_t weight = 10;
  std::unordered_map<std::string, DesignEntityType> declarations;
  PqlToken first_arg;
  PqlToken second_arg;
  const pkb::PkbPtr &pkb;
  using handler = Table (NextClause::*)();
  const std::map<std::pair<PqlTokenType, PqlTokenType>, handler> execute_handler{
      {{PqlTokenType::SYNONYM, PqlTokenType::SYNONYM}, &NextClause::HandleSynonymSynonym},
      {{PqlTokenType::SYNONYM, PqlTokenType::UNDERSCORE}, &NextClause::HandleSynonymWildcard},
      {{PqlTokenType::SYNONYM, PqlTokenType::NUMBER}, &NextClause::HandleSynonymInteger},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::SYNONYM}, &NextClause::HandleWildcardSynonym},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::UNDERSCORE}, &NextClause::HandleWildcardWildcard},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::NUMBER}, &NextClause::HandleWildcardInteger},
      {{PqlTokenType::NUMBER, PqlTokenType::SYNONYM}, &NextClause::HandleIntegerSynonym},
      {{PqlTokenType::NUMBER, PqlTokenType::UNDERSCORE}, &NextClause::HandleIntegerWildcard},
      {{PqlTokenType::NUMBER, PqlTokenType::NUMBER}, &NextClause::HandleIntegerInteger},

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
  using bool_handler = bool (NextClause::*)();
  const std::map<std::pair<PqlTokenType, PqlTokenType>, bool_handler> execute_bool_handler{
      {{PqlTokenType::SYNONYM, PqlTokenType::SYNONYM}, &NextClause::HandleSynonymSynonymBool},
      {{PqlTokenType::SYNONYM, PqlTokenType::UNDERSCORE}, &NextClause::HandleSynonymWildcardBool},
      {{PqlTokenType::SYNONYM, PqlTokenType::NUMBER}, &NextClause::HandleSynonymIntegerBool},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::SYNONYM}, &NextClause::HandleWildcardSynonymBool},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::UNDERSCORE}, &NextClause::HandleWildcardWildcardBool},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::NUMBER}, &NextClause::HandleWildcardIntegerBool},
      {{PqlTokenType::NUMBER, PqlTokenType::SYNONYM}, &NextClause::HandleIntegerSynonymBool},
      {{PqlTokenType::NUMBER, PqlTokenType::UNDERSCORE}, &NextClause::HandleIntegerWildcardBool},
      {{PqlTokenType::NUMBER, PqlTokenType::NUMBER}, &NextClause::HandleIntegerIntegerBool},

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

#endif //NEXT_CLAUSE_H_
