#ifndef NEXTT_CLAUSE_H_
#define NEXTT_CLAUSE_H_

#include "clause.h"

namespace pql {

class NextTClause : public Clause {
 public:
  NextTClause(const std::unordered_map<std::string, DesignEntityType> &declarations,
              const PqlToken &first_arg,
              const PqlToken &second_arg,
              const PkbPtr &pkb);
  Table Execute() override;
  bool ExecuteBool() override;
  std::set<std::string> GetSynonyms() override;
  size_t GetSynonymsSize() override;
 private:
  std::unordered_map<std::string, DesignEntityType> declarations;
  PqlToken first_arg;
  PqlToken second_arg;
  const PkbPtr &pkb;
  using handler = Table (NextTClause::*)();
  const std::map<std::pair<PqlTokenType, PqlTokenType>, handler> execute_handler{
      {{PqlTokenType::SYNONYM, PqlTokenType::SYNONYM}, &NextTClause::HandleSynonymSynonym},
      {{PqlTokenType::SYNONYM, PqlTokenType::UNDERSCORE}, &NextTClause::HandleSynonymWildcard},
      {{PqlTokenType::SYNONYM, PqlTokenType::NUMBER}, &NextTClause::HandleSynonymInteger},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::SYNONYM}, &NextTClause::HandleWildcardSynonym},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::UNDERSCORE}, &NextTClause::HandleWildcardWildcard},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::NUMBER}, &NextTClause::HandleWildcardInteger},
      {{PqlTokenType::NUMBER, PqlTokenType::SYNONYM}, &NextTClause::HandleIntegerSynonym},
      {{PqlTokenType::NUMBER, PqlTokenType::UNDERSCORE}, &NextTClause::HandleIntegerWildcard},
      {{PqlTokenType::NUMBER, PqlTokenType::NUMBER}, &NextTClause::HandleIntegerInteger},

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
  using bool_handler = bool (NextTClause::*)();
  const std::map<std::pair<PqlTokenType, PqlTokenType>, bool_handler> execute_bool_handler{
      {{PqlTokenType::SYNONYM, PqlTokenType::SYNONYM}, &NextTClause::HandleSynonymSynonymBool},
      {{PqlTokenType::SYNONYM, PqlTokenType::UNDERSCORE}, &NextTClause::HandleSynonymWildcardBool},
      {{PqlTokenType::SYNONYM, PqlTokenType::NUMBER}, &NextTClause::HandleSynonymIntegerBool},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::SYNONYM}, &NextTClause::HandleWildcardSynonymBool},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::UNDERSCORE}, &NextTClause::HandleWildcardWildcardBool},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::NUMBER}, &NextTClause::HandleWildcardIntegerBool},
      {{PqlTokenType::NUMBER, PqlTokenType::SYNONYM}, &NextTClause::HandleIntegerSynonymBool},
      {{PqlTokenType::NUMBER, PqlTokenType::UNDERSCORE}, &NextTClause::HandleIntegerWildcardBool},
      {{PqlTokenType::NUMBER, PqlTokenType::NUMBER}, &NextTClause::HandleIntegerIntegerBool},

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

#endif //NEXTT_CLAUSE_H_
