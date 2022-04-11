#include "select_clause.h"

namespace pql {

using namespace clause_util;

SelectClause::SelectClause(const PqlToken &result_clause,
                           const std::unordered_map<std::string, DesignEntityType> &declarations,
                           const pkb::PkbPtr &pkb)
    : result_clause(result_clause), declarations(declarations), pkb(pkb) {}

Table SelectClause::Execute() {
  std::unordered_set<std::string> single_constraints;
  if (IsArgAttribute(result_clause)) {
    std::pair<std::pair<DesignEntityType, std::string>, AttriName>
        attribute = Utils::ParseAttributeRef(result_clause, declarations);
    single_constraints = pkb->GetStmt(clause_util::GetStmtType(attribute.first.first));
    Table table;
    if (table_synonyms.find(attribute.first.second)!=table_synonyms.end()) {
      table.ToggleAttributeResult();
      return table;
    }
    table = std::move(Table(attribute.first.second, single_constraints));
    table.ToggleAttributeResult();
    return table;
  } else {
    single_constraints =
        pkb->GetStmt(clause_util::GetStmtType(clause_util::GetSynonymDesignEntity(result_clause, declarations)));
    Table table;
    if (table_synonyms.find(result_clause.value)!=table_synonyms.end()) {
      table.ToggleSynonymResult();
      return table;
    }
    table = std::move(Table(result_clause.value, single_constraints));
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

size_t SelectClause::GetWeight() {
  // select clauses never inside any of the groups that require sorting
  return 0;
}

SelectClause::SelectClause(const std::unordered_set<std::string> &table_synonyms,
                           const PqlToken &result_clause,
                           const std::unordered_map<std::string, DesignEntityType> &declarations,
                           const pkb::PkbPtr &pkb)
    : table_synonyms(table_synonyms), result_clause(result_clause), declarations(declarations), pkb(pkb) {}

}
