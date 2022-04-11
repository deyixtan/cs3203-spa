#include "select_boolean_clause.h"

namespace pql {

Table SelectBooleanClause::Execute() {
  Table table;
  table.ToggleBooleanResult();
  return table;
}

bool SelectBooleanClause::ExecuteBool() {
  return false;
}

std::set<std::string> SelectBooleanClause::GetSynonyms() {
  return {};
}

size_t SelectBooleanClause::GetSynonymsSize() {
  size_t size = 0;
  return size;
}

size_t SelectBooleanClause::GetWeight() {
  // select clauses never inside any of the groups that require sorting
  return 0;
}

}
