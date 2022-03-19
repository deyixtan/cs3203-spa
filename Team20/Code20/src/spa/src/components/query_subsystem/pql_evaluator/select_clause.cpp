#include "select_clause.h"

namespace pql {

SelectClause::SelectClause(const PqlToken &result_clause, const std::unordered_map<std::string, DesignEntityType> &declarations_, PKB *pkb_)
    : result_clause(result_clause), declarations(declarations_), pkb(pkb_) {}

Table SelectClause::Execute() {
  std::unordered_set<std::string> single_constraints;
  if (result_clause.type == PqlTokenType::ATTRIBUTE) {
    std::pair<std::pair<DesignEntityType, std::string>, AtrriName> attribute = Utils::ParseAttributeRef(result_clause, declarations);
    single_constraints = pkb->GetStmt(clause_util::GetStmtType(attribute.first.first));
    Table table = Table(attribute.first.second, single_constraints);
    table.ToggleAttributeResult();
    return table;
  } else {
    single_constraints = pkb->GetStmt(clause_util::GetStmtType(GetSynonymDesignEntity(result_clause)));
  }
  return {result_clause.value, single_constraints};
}

DesignEntityType SelectClause::GetSynonymDesignEntity(const PqlToken &arg) {
  assert(arg.type==PqlTokenType::SYNONYM);
  for (auto declaration : declarations) {
    if (arg.value==declaration.first) {
      return declaration.second;
    }
  }

  throw std::out_of_range("Synonym not declared");
}

}
