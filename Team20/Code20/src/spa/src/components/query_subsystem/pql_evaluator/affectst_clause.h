#ifndef AFFECTST_CLAUSE_H_
#define AFFECTST_CLAUSE_H_

#include <map>

#include "clause.h"
#include "components/query_subsystem/utils.h"
#include "components/pkb/pkb.h"

namespace pql {

class AffectsTClause : public Clause {
 public:
  AffectsTClause(const PqlToken &first_arg,
                 const PqlToken &second_arg,
                 const pkb::PkbPtr &pkb);
  Table Execute() override;
  bool ExecuteBool() override;
  std::set<std::string> GetSynonyms() override;
  size_t GetSynonymsSize() override;
  size_t GetWeight() override;
 private:
  size_t weight = 28;
  PqlToken first_arg;
  PqlToken second_arg;
  const pkb::PkbPtr &pkb;
  using handler = Table (AffectsTClause::*)();
  const std::map<std::pair<PqlTokenType, PqlTokenType>, handler> execute_handler{
      {{PqlTokenType::SYNONYM, PqlTokenType::SYNONYM}, &AffectsTClause::HandleSynonymSynonym},
      {{PqlTokenType::SYNONYM, PqlTokenType::UNDERSCORE}, &AffectsTClause::HandleSynonymWildcard},
      {{PqlTokenType::SYNONYM, PqlTokenType::NUMBER}, &AffectsTClause::HandleSynonymInteger},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::SYNONYM}, &AffectsTClause::HandleWildcardSynonym},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::UNDERSCORE}, &AffectsTClause::HandleWildcardWildcard},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::NUMBER}, &AffectsTClause::HandleWildcardInteger},
      {{PqlTokenType::NUMBER, PqlTokenType::SYNONYM}, &AffectsTClause::HandleIntegerSynonym},
      {{PqlTokenType::NUMBER, PqlTokenType::UNDERSCORE}, &AffectsTClause::HandleIntegerWildcard},
      {{PqlTokenType::NUMBER, PqlTokenType::NUMBER}, &AffectsTClause::HandleIntegerInteger},

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
  using bool_handler = bool (AffectsTClause::*)();
  const std::map<std::pair<PqlTokenType, PqlTokenType>, bool_handler> execute_bool_handler{
      {{PqlTokenType::SYNONYM, PqlTokenType::SYNONYM}, &AffectsTClause::HandleSynonymSynonymBool},
      {{PqlTokenType::SYNONYM, PqlTokenType::UNDERSCORE}, &AffectsTClause::HandleSynonymWildcardBool},
      {{PqlTokenType::SYNONYM, PqlTokenType::NUMBER}, &AffectsTClause::HandleSynonymIntegerBool},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::SYNONYM}, &AffectsTClause::HandleWildcardSynonymBool},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::UNDERSCORE}, &AffectsTClause::HandleWildcardWildcardBool},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::NUMBER}, &AffectsTClause::HandleWildcardIntegerBool},
      {{PqlTokenType::NUMBER, PqlTokenType::SYNONYM}, &AffectsTClause::HandleIntegerSynonymBool},
      {{PqlTokenType::NUMBER, PqlTokenType::UNDERSCORE}, &AffectsTClause::HandleIntegerWildcardBool},
      {{PqlTokenType::NUMBER, PqlTokenType::NUMBER}, &AffectsTClause::HandleIntegerIntegerBool},

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
#endif //AFFECTST_CLAUSE_H_
