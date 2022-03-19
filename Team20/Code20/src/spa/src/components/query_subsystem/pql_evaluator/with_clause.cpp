#include "with_clause.h"
#include "clause_util.h"

namespace pql {

using namespace clause_util;

WithClause::WithClause(const std::unordered_map<std::string, DesignEntityType> &declarations,
           const PqlToken &first_arg,
           const PqlToken &second_arg,
           PKB *pkb) : declarations(declarations), first_arg(first_arg), second_arg(second_arg), pkb(pkb) {}
Table WithClause::Execute() {
  if (IsArgAttribute(first_arg) && IsArgAttribute(second_arg)) {
    // with s.stmt# = c.value || v.varName = p.procName
    return HandleAttributeAttribute();
  } else if (IsArgAttribute(first_arg) && IsArgInteger(second_arg)) {
    // with s.stmt# = 1
    return HandleAttributeInteger();
  } else if (IsArgAttribute(first_arg) && IsArgIdent(second_arg)) {
    // with v.varName = "x"
    return HandleAttributeIdent();
  } else if (IsArgInteger(first_arg) && IsArgAttribute(second_arg)) {
    // with 1 = s.stmt#
    return HandleIntegerAttribute();
  } else if (IsArgInteger(first_arg) && IsArgInteger(second_arg)) {
    // with 1 = 1
    return HandleIntegerInteger();
  } else if (IsArgIdent(first_arg) && IsArgAttribute(second_arg)) {
    // "x" = v.varName
    return HandleIdentAttribute();
  } else if (IsArgIdent(first_arg) && IsArgIdent(second_arg)) {
    // "x" = "x"
    return HandleIdentIdent();
  }
}

Table WithClause::HandleAttributeAttribute() {
  std::pair<std::pair<DesignEntityType, std::string>, AtrriName> first_attribute = Utils::ParseAttributeRef(first_arg, declarations);
  std::pair<std::pair<DesignEntityType, std::string>, AtrriName> second_attribute = Utils::ParseAttributeRef(second_arg, declarations);
  auto single_constraints_first = pkb->GetStmt(GetStmtType(first_attribute.first.first));
  auto single_constraints_second = pkb->GetStmt(GetStmtType(first_attribute.first.first));

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> intersection_pair = HandleSetIntersection(single_constraints_first, single_constraints_second);

  Table table;
  if (intersection_pair.empty()) {
    table.EncounteredFalseClause();
    return table;
  }

  return {first_attribute.first.second, second_attribute.first.second, intersection_pair};
}

Table WithClause::HandleAttributeInteger() {
  std::pair<std::pair<DesignEntityType, std::string>, AtrriName> first_attribute = Utils::ParseAttributeRef(first_arg, declarations);
  auto single_constraints = pkb->GetStmt(GetStmtType(first_attribute.first.first));
  Table table;
  if (single_constraints.count(second_arg.value) == 0) {
    table.EncounteredFalseClause();
    return table;
  }
  std::unordered_set<std::string> result_set;
  result_set.insert(second_arg.value);
  return {first_attribute.first.second, result_set};
}

Table WithClause::HandleAttributeIdent() {
  return HandleAttributeInteger();
}

Table WithClause::HandleIntegerAttribute() {
  std::pair<std::pair<DesignEntityType, std::string>, AtrriName> second_attribute = Utils::ParseAttributeRef(second_arg, declarations);
  auto single_constraints = pkb->GetStmt(GetStmtType(second_attribute.first.first));
  Table table;
  if (single_constraints.count(first_arg.value) == 0) {
    table.EncounteredFalseClause();
    return table;
  }
  std::unordered_set<std::string> result_set;
  result_set.insert(first_arg.value);
  return {second_attribute.first.second, result_set};
}

Table WithClause::HandleIntegerInteger() {
  Table table;
  if (first_arg.value != second_arg.value) {
    table.EncounteredFalseClause();
  }
  return table;
}

Table WithClause::HandleIdentAttribute() {
  return HandleIntegerAttribute();
}

Table WithClause::HandleIdentIdent() {
  // same logic as HandleIntegerInteger
  return HandleIntegerInteger();
}

std::unordered_set<std::pair<std::string, std::string>, pair_hash> WithClause::HandleSetIntersection(std::unordered_set<std::string> set_one, std::unordered_set<std::string> set_two) {
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> intersection_pair;
  for (auto s : set_one) {
    if (set_two.count(s) != 0) {
      intersection_pair.insert(std::make_pair(s, s));
    }
  }

  return intersection_pair;
}

}