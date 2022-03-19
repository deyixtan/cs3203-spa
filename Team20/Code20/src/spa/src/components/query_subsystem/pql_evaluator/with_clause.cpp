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

  std::unordered_set<std::string> single_constraints_first;
  std::unordered_set<std::string> single_constraints_second;

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> intersection_pair;
  bool is_conversion_needed_first = Utils::IsConversionNeeded(first_attribute.first.first, first_attribute.second);
  bool is_conversion_needed_second = Utils::IsConversionNeeded(second_attribute.first.first, second_attribute.second);

  if (is_conversion_needed_first && is_conversion_needed_second) {
    single_constraints_first = pkb->GetName(GetStmtType(first_attribute.first.first));
    single_constraints_second = pkb->GetName(GetStmtType(second_attribute.first.first));
    std::unordered_set<std::string> single_column_intersection = HandleSetIntersectionSingleColumn(single_constraints_first, single_constraints_second);
    std::unordered_set<std::string> intersection_first;
    std::unordered_set<std::string> intersection_second;
    for (auto s : single_column_intersection) {
      std::unordered_set<std::string> all_stmts_first = pkb->GetStmtByName(GetStmtType(first_attribute.first.first), s);
      std::unordered_set<std::string> all_stmts_second = pkb->GetStmtByName(GetStmtType(second_attribute.first.first), s);
      intersection_first.insert(all_stmts_first.begin(), all_stmts_first.end());
      intersection_second.insert(all_stmts_second.begin(), all_stmts_second.end());
    }

    std::unordered_set<std::string>::iterator it1 = intersection_first.begin();
    std::unordered_set<std::string>::iterator it2 = intersection_second.begin();
    for(; it1 != intersection_first.end() && it2 != intersection_second.end(); ++it1, ++it2) {
        intersection_pair.insert(std::make_pair(*it1, *it2));
    }
  } else if (is_conversion_needed_first) {
    single_constraints_first = pkb->GetName(GetStmtType(first_attribute.first.first));
    single_constraints_second = pkb->GetStmt(GetStmtType(second_attribute.first.first));
    std::unordered_set<std::string> single_column_intersection = HandleSetIntersectionSingleColumn(single_constraints_first, single_constraints_second);
    std::unordered_set<std::string> intersection_temp;
    for (auto s : single_column_intersection) {
      std::unordered_set<std::string> all_stmts = pkb->GetStmtByName(GetStmtType(first_attribute.first.first), s);
      for (auto stmt_no : all_stmts) {
        intersection_pair.insert(std::make_pair(stmt_no, s));
      }
    }

  } else if (is_conversion_needed_second) {
    single_constraints_second = pkb->GetName(GetStmtType(second_attribute.first.first));
    single_constraints_first = pkb->GetStmt(GetStmtType(first_attribute.first.first));
    std::unordered_set<std::string> single_column_intersection = HandleSetIntersectionSingleColumn(single_constraints_first, single_constraints_second);
    std::unordered_set<std::string> intersection_temp;
    for (auto s : single_column_intersection) {
      std::unordered_set<std::string> all_stmts = pkb->GetStmtByName(GetStmtType(second_attribute.first.first), s);
      for (auto stmt_no : all_stmts) {
        intersection_pair.insert(std::make_pair(s, stmt_no));
      }
    }
  } else {
    single_constraints_first = pkb->GetStmt(GetStmtType(first_attribute.first.first));
    single_constraints_second = pkb->GetStmt(GetStmtType(second_attribute.first.first));
    intersection_pair = HandleSetIntersection(single_constraints_first, single_constraints_second);
  }

  Table table;
  if (intersection_pair.empty()) {
    table.EncounteredFalseClause();
    return table;
  }

  return {first_attribute.first.second, second_attribute.first.second, intersection_pair};
}

Table WithClause::HandleAttributeIdent() {
  std::pair<std::pair<DesignEntityType, std::string>, AtrriName> first_attribute = Utils::ParseAttributeRef(first_arg, declarations);
  std::unordered_set<std::string> single_constraints;
  Table table;
  bool is_conversion_needed = Utils::IsConversionNeeded(first_attribute.first.first, first_attribute.second);
  if (is_conversion_needed) {
    single_constraints = pkb->GetStmtByName(GetStmtType(first_attribute.first.first), second_arg.value);
    if (single_constraints.empty()) {
      table.EncounteredFalseClause();
      return table;
    }
    return {first_attribute.first.second, single_constraints};
  } else {
    single_constraints = pkb->GetStmt(GetStmtType(first_attribute.first.first));
    if (single_constraints.count(second_arg.value) == 0) {
      table.EncounteredFalseClause();
      return table;
    }
  }

  std::unordered_set<std::string> result_set;
  result_set.insert(second_arg.value);
  return {first_attribute.first.second, result_set};
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
  std::pair<std::pair<DesignEntityType, std::string>, AtrriName> second_attribute = Utils::ParseAttributeRef(second_arg, declarations);
  std::unordered_set<std::string> single_constraints; single_constraints;
  Table table;
  bool is_conversion_needed = Utils::IsConversionNeeded(second_attribute.first.first, second_attribute.second);
  if (is_conversion_needed) {
    single_constraints = pkb->GetStmtByName(GetStmtType(second_attribute.first.first), first_arg.value);
    if (single_constraints.empty()) {
      table.EncounteredFalseClause();
      return table;
    }
    return {second_attribute.first.second, single_constraints};
  } else {
    single_constraints = pkb->GetStmt(GetStmtType(second_attribute.first.first));
    if (single_constraints.count(first_arg.value) == 0) {
      table.EncounteredFalseClause();
      return table;
    }
  }
  std::unordered_set<std::string> result_set;
  result_set.insert(first_arg.value);
  return {second_attribute.first.second, result_set};
}

Table WithClause::HandleIdentIdent() {
  // same logic as HandleIntegerInteger
  return HandleIntegerInteger();
}

std::unordered_set<std::string> WithClause::HandleSetIntersectionSingleColumn(std::unordered_set<std::string> set_one, std::unordered_set<std::string> set_two) {
  std::unordered_set<std::string> intersection_pair;
  for (auto s : set_one) {
    if (set_two.count(s) != 0) {
      intersection_pair.insert(s);
    }
  }
  return intersection_pair;
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