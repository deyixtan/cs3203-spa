#ifndef WITH_CLAUSE_H
#define WITH_CLAUSE_H

#include "clause.h"
#include "components/pkb/pkb.h"
#include "components/query_subsystem/pql_parser/parsed_query.h"
#include "../utils.h"

namespace pql {

class WithClause : public Clause {
 public:
  WithClause(const std::unordered_map<std::string, DesignEntityType> &declarations,
             const PqlToken &first_arg,
             const PqlToken &second_arg,
             const pkb::PkbPtr &pkb);
  Table Execute() override;
  bool ExecuteBool() override;
  std::set<std::string> GetSynonyms() override;
  size_t GetSynonymsSize() override;
  size_t GetWeight() override;
 private:
  size_t weight = 1;
  std::unordered_map<std::string, DesignEntityType> declarations;
  PqlToken first_arg;
  PqlToken second_arg;
  const pkb::PkbPtr &pkb;
  using handler = Table (WithClause::*)();
  const std::map<std::pair<PqlTokenType, PqlTokenType>, handler> execute_handler{
      {{PqlTokenType::ATTRIBUTE, PqlTokenType::ATTRIBUTE}, &WithClause::HandleAttributeAttribute},
      {{PqlTokenType::ATTRIBUTE, PqlTokenType::NUMBER}, &WithClause::HandleAttributeInteger},
      {{PqlTokenType::ATTRIBUTE, PqlTokenType::IDENT_WITH_QUOTES}, &WithClause::HandleAttributeIdent},
      {{PqlTokenType::NUMBER, PqlTokenType::ATTRIBUTE}, &WithClause::HandleIntegerAttribute},
      {{PqlTokenType::NUMBER, PqlTokenType::NUMBER}, &WithClause::HandleIntegerInteger},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::ATTRIBUTE}, &WithClause::HandleIdentAttribute},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::IDENT_WITH_QUOTES}, &WithClause::HandleIdentIdent}
  };
  Table HandleAttributeAttribute();
  Table HandleAttributeInteger();
  Table HandleAttributeIdent();
  Table HandleIntegerAttribute();
  Table HandleIntegerInteger();
  Table HandleIdentAttribute();
  Table HandleIdentIdent();
  using bool_handler = bool (WithClause::*)();
  const std::map<std::pair<PqlTokenType, PqlTokenType>, bool_handler> execute_bool_handler{
      {{PqlTokenType::ATTRIBUTE, PqlTokenType::ATTRIBUTE}, &WithClause::HandleAttributeAttributeBool},
      {{PqlTokenType::ATTRIBUTE, PqlTokenType::NUMBER}, &WithClause::HandleAttributeIntegerBool},
      {{PqlTokenType::ATTRIBUTE, PqlTokenType::IDENT_WITH_QUOTES}, &WithClause::HandleAttributeIdentBool},
      {{PqlTokenType::NUMBER, PqlTokenType::ATTRIBUTE}, &WithClause::HandleIntegerAttributeBool},
      {{PqlTokenType::NUMBER, PqlTokenType::NUMBER}, &WithClause::HandleIntegerIntegerBool},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::ATTRIBUTE}, &WithClause::HandleIdentAttributeBool},
      {{PqlTokenType::IDENT_WITH_QUOTES, PqlTokenType::IDENT_WITH_QUOTES}, &WithClause::HandleIdentIdentBool}
  };
  bool HandleAttributeAttributeBool();
  bool HandleAttributeIntegerBool();
  bool HandleAttributeIdentBool();
  bool HandleIntegerAttributeBool();
  bool HandleIntegerIntegerBool();
  bool HandleIdentAttributeBool();
  bool HandleIdentIdentBool();
  static std::unordered_set<std::string> HandleSetIntersectionSingleColumn(std::unordered_set<std::string>,
                                                                           std::unordered_set<std::string>);
  static std::unordered_set<std::pair<std::string, std::string>,
                            pair_hash> HandleSetIntersection(const std::unordered_set<std::string> &,
                                                             const std::unordered_set<std::string> &);
};

}

#endif // WITH_CLAUSE_H
