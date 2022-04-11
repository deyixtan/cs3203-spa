#ifndef FOLLOWS_CLAUSE_H_
#define FOLLOWS_CLAUSE_H_

#include <map>

#include "components/query_subsystem/pql_evaluator/clauses/clause.h"
#include "components/pkb/pkb.h"
#include "components/query_subsystem/pql_parser/parsed_query.h"

namespace pql {

class FollowsClause : public Clause {
 public:
  FollowsClause(const std::unordered_map<std::string, DesignEntityType> &declarations,
                const PqlToken &first_arg,
                const PqlToken &second_arg,
                const pkb::PkbPtr &pkb);
  Table Execute() override;
  bool ExecuteBool() override;
  std::set<std::string> GetSynonyms() override;
  size_t GetSynonymsSize() override;
  size_t GetWeight() override;
 private:
  size_t weight = 3;
  std::unordered_map<std::string, DesignEntityType> declarations;
  PqlToken first_arg;
  PqlToken second_arg;
  const pkb::PkbPtr &pkb;
  using handler = Table (FollowsClause::*)();
  const std::map<std::pair<PqlTokenType, PqlTokenType>, handler> execute_handler{
      {{PqlTokenType::SYNONYM, PqlTokenType::SYNONYM}, &FollowsClause::HandleSynonymSynonym},
      {{PqlTokenType::SYNONYM, PqlTokenType::UNDERSCORE}, &FollowsClause::HandleSynonymWildcard},
      {{PqlTokenType::SYNONYM, PqlTokenType::NUMBER}, &FollowsClause::HandleSynonymInteger},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::SYNONYM}, &FollowsClause::HandleWildcardSynonym},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::UNDERSCORE}, &FollowsClause::HandleWildcardWildcard},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::NUMBER}, &FollowsClause::HandleWildcardInteger},
      {{PqlTokenType::NUMBER, PqlTokenType::SYNONYM}, &FollowsClause::HandleIntegerSynonym},
      {{PqlTokenType::NUMBER, PqlTokenType::UNDERSCORE}, &FollowsClause::HandleIntegerWildcard},
      {{PqlTokenType::NUMBER, PqlTokenType::NUMBER}, &FollowsClause::HandleIntegerInteger},

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
  using bool_handler = bool (FollowsClause::*)();
  const std::map<std::pair<PqlTokenType, PqlTokenType>, bool_handler> execute_bool_handler{
      {{PqlTokenType::SYNONYM, PqlTokenType::SYNONYM}, &FollowsClause::HandleSynonymSynonymBool},
      {{PqlTokenType::SYNONYM, PqlTokenType::UNDERSCORE}, &FollowsClause::HandleSynonymWildcardBool},
      {{PqlTokenType::SYNONYM, PqlTokenType::NUMBER}, &FollowsClause::HandleSynonymIntegerBool},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::SYNONYM}, &FollowsClause::HandleWildcardSynonymBool},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::UNDERSCORE}, &FollowsClause::HandleWildcardWildcardBool},
      {{PqlTokenType::UNDERSCORE, PqlTokenType::NUMBER}, &FollowsClause::HandleWildcardIntegerBool},
      {{PqlTokenType::NUMBER, PqlTokenType::SYNONYM}, &FollowsClause::HandleIntegerSynonymBool},
      {{PqlTokenType::NUMBER, PqlTokenType::UNDERSCORE}, &FollowsClause::HandleIntegerWildcardBool},
      {{PqlTokenType::NUMBER, PqlTokenType::NUMBER}, &FollowsClause::HandleIntegerIntegerBool},

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

#endif //FOLLOWS_CLAUSE_H_
