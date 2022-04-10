#include "select_clause.h"

namespace pql {

using namespace clause_util;

SelectClause::SelectClause(const PqlToken &result_clause,
                           const std::unordered_map<std::string, DesignEntityType> &declarations_,
                           const PkbPtr &pkb_)
    : result_clause(result_clause), declarations(declarations_), pkb(pkb_) {}

Table SelectClause::Execute() {
  std::unordered_set<std::string> single_constraints;
  if (IsArgAttribute(result_clause)) {
    std::pair<std::pair<DesignEntityType, std::string>, AttriName>
        attribute = Utils::ParseAttributeRef(result_clause, declarations);
    single_constraints = pkb->GetStmt(clause_util::GetStmtType(attribute.first.first));
    Table table = Table(attribute.first.second, single_constraints);
    table.ToggleAttributeResult();
    return table;
  } else {
    single_constraints =
        pkb->GetStmt(clause_util::GetStmtType(clause_util::GetSynonymDesignEntity(result_clause, declarations)));
    Table table = Table(result_clause.value, single_constraints);
    table.ToggleSynonymResult();
    return table;
  }
}

bool SelectClause::ExecuteBool() {
  return false;
}

std::set<std::string> SelectClause::GetSynonyms() {
  std::set<std::string> synonyms;
  if (IsArgSynonym(result_clause)) {
    synonyms.emplace(result_clause.value);
  }
  if (IsArgAttribute(result_clause)) {
    auto attribute = Utils::ParseAttributeRef(result_clause, declarations);
    synonyms.emplace(attribute.first.second);
  }

  return synonyms;
}

size_t SelectClause::GetSynonymsSize() {
  size_t size = 0;
  if (IsArgSynonym(result_clause)) {
    size++;
  }
  if (IsArgAttribute(result_clause)) {
    size++;
  }

  return size;
}

}
