#include "with_clause.h"
#include "clause_util.h"

namespace pql {

using namespace clause_util;

WithClause::WithClause(const std::unordered_map<std::string, DesignEntityType> &declarations,
                       const PqlToken &first_arg,
                       const PqlToken &second_arg,
                       const PkbPtr &pkb)
    : declarations(declarations), first_arg(first_arg), second_arg(second_arg), pkb(pkb) {}

Table WithClause::Execute() {
  return (this->*execute_handler.at({first_arg.type, second_arg.type}))();
}

Table WithClause::HandleAttributeAttribute() {
  std::pair<std::pair<DesignEntityType, std::string>, AttriName>
      first_attribute = Utils::ParseAttributeRef(first_arg, declarations);
  std::pair<std::pair<DesignEntityType, std::string>, AttriName>
      second_attribute = Utils::ParseAttributeRef(second_arg, declarations);

  std::unordered_set<std::string> single_constraints_first;
  std::unordered_set<std::string> single_constraints_second;

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> intersection_pair;
  bool is_conversion_needed_first = Utils::IsConversionNeeded(first_attribute.first.first, first_attribute.second);
  bool is_conversion_needed_second = Utils::IsConversionNeeded(second_attribute.first.first, second_attribute.second);

  if (is_conversion_needed_first && is_conversion_needed_second) {
    single_constraints_first = pkb->GetName(GetStmtType(first_attribute.first.first));
    single_constraints_second = pkb->GetName(GetStmtType(second_attribute.first.first));
    std::unordered_set<std::string> single_column_intersection =
        HandleSetIntersectionSingleColumn(single_constraints_first, single_constraints_second);
    std::unordered_set<std::string> intersection_first;
    std::unordered_set<std::string> intersection_second;
    for (const auto &s : single_column_intersection) {
      std::unordered_set<std::string> all_stmts_first = pkb->GetStmtByName(GetStmtType(first_attribute.first.first), s);
      std::unordered_set<std::string>
          all_stmts_second = pkb->GetStmtByName(GetStmtType(second_attribute.first.first), s);
      intersection_first.insert(all_stmts_first.begin(), all_stmts_first.end());
      intersection_second.insert(all_stmts_second.begin(), all_stmts_second.end());
    }

    auto it1 = intersection_first.begin();
    auto it2 = intersection_second.begin();
    for (; it1!=intersection_first.end() && it2!=intersection_second.end(); ++it1, ++it2) {
      intersection_pair.insert(std::make_pair(*it1, *it2));
    }
  } else if (is_conversion_needed_first) {
    single_constraints_first = pkb->GetName(GetStmtType(first_attribute.first.first));
    single_constraints_second = pkb->GetStmt(GetStmtType(second_attribute.first.first));
    std::unordered_set<std::string> single_column_intersection =
        HandleSetIntersectionSingleColumn(single_constraints_first, single_constraints_second);
    std::unordered_set<std::string> intersection_temp;
    for (const auto &s : single_column_intersection) {
      std::unordered_set<std::string> all_stmts = pkb->GetStmtByName(GetStmtType(first_attribute.first.first), s);
      for (const auto &stmt_no : all_stmts) {
        intersection_pair.insert(std::make_pair(stmt_no, s));
      }
    }

  } else if (is_conversion_needed_second) {
    single_constraints_second = pkb->GetName(GetStmtType(second_attribute.first.first));
    single_constraints_first = pkb->GetStmt(GetStmtType(first_attribute.first.first));
    std::unordered_set<std::string> single_column_intersection =
        HandleSetIntersectionSingleColumn(single_constraints_first, single_constraints_second);
    std::unordered_set<std::string> intersection_temp;
    for (const auto &s : single_column_intersection) {
      std::unordered_set<std::string> all_stmts = pkb->GetStmtByName(GetStmtType(second_attribute.first.first), s);
      for (const auto &stmt_no : all_stmts) {
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
    table.ToggleFalseClause();
    return table;
  }

  return {first_attribute.first.second, second_attribute.first.second, intersection_pair};
}

Table WithClause::HandleAttributeIdent() {
  std::pair<std::pair<DesignEntityType, std::string>, AttriName>
      first_attribute = Utils::ParseAttributeRef(first_arg, declarations);
  std::unordered_set<std::string> single_constraints;
  Table table;
  bool is_conversion_needed = Utils::IsConversionNeeded(first_attribute.first.first, first_attribute.second);
  if (is_conversion_needed) {
    single_constraints = pkb->GetStmtByName(GetStmtType(first_attribute.first.first), second_arg.value);
    if (single_constraints.empty()) {
      table.ToggleFalseClause();
      return table;
    }
    return {first_attribute.first.second, single_constraints};
  } else {
    single_constraints = pkb->GetStmt(GetStmtType(first_attribute.first.first));
    if (single_constraints.count(second_arg.value)==0) {
      table.ToggleFalseClause();
      return table;
    }
  }

  std::unordered_set<std::string> result_set;
  result_set.insert(second_arg.value);
  return {first_attribute.first.second, result_set};
}

Table WithClause::HandleAttributeInteger() {
  std::pair<std::pair<DesignEntityType, std::string>, AttriName>
      first_attribute = Utils::ParseAttributeRef(first_arg, declarations);
  auto single_constraints = pkb->GetStmt(GetStmtType(first_attribute.first.first));
  Table table;
  if (single_constraints.count(second_arg.value)==0) {
    table.ToggleFalseClause();
    return table;
  }
  std::unordered_set<std::string> result_set;
  result_set.insert(second_arg.value);
  return {first_attribute.first.second, result_set};
}

Table WithClause::HandleIntegerAttribute() {
  std::pair<std::pair<DesignEntityType, std::string>, AttriName>
      second_attribute = Utils::ParseAttributeRef(second_arg, declarations);
  auto single_constraints = pkb->GetStmt(GetStmtType(second_attribute.first.first));
  Table table;
  if (single_constraints.count(first_arg.value)==0) {
    table.ToggleFalseClause();
    return table;
  }
  std::unordered_set<std::string> result_set;
  result_set.insert(first_arg.value);
  return {second_attribute.first.second, result_set};
}

Table WithClause::HandleIntegerInteger() {
  Table table;
  if (first_arg.value!=second_arg.value) {
    table.ToggleFalseClause();
  }
  return table;
}

Table WithClause::HandleIdentAttribute() {
  std::pair<std::pair<DesignEntityType, std::string>, AttriName>
      second_attribute = Utils::ParseAttributeRef(second_arg, declarations);
  std::unordered_set<std::string> single_constraints;
  Table table;
  bool is_conversion_needed = Utils::IsConversionNeeded(second_attribute.first.first, second_attribute.second);
  if (is_conversion_needed) {
    single_constraints = pkb->GetStmtByName(GetStmtType(second_attribute.first.first), first_arg.value);
    if (single_constraints.empty()) {
      table.ToggleFalseClause();
      return table;
    }
    return {second_attribute.first.second, single_constraints};
  } else {
    single_constraints = pkb->GetStmt(GetStmtType(second_attribute.first.first));
    if (single_constraints.count(first_arg.value)==0) {
      table.ToggleFalseClause();
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

bool WithClause::ExecuteBool() {
  return (this->*execute_bool_handler.at({first_arg.type, second_arg.type}))();
}

bool WithClause::HandleAttributeAttributeBool() {
  std::pair<std::pair<DesignEntityType, std::string>, AttriName>
      first_attribute = Utils::ParseAttributeRef(first_arg, declarations);
  std::pair<std::pair<DesignEntityType, std::string>, AttriName>
      second_attribute = Utils::ParseAttributeRef(second_arg, declarations);

  std::unordered_set<std::string> single_constraints_first;
  std::unordered_set<std::string> single_constraints_second;

  std::unordered_set<std::pair<std::string, std::string>, pair_hash> intersection_pair;
  bool is_conversion_needed_first = Utils::IsConversionNeeded(first_attribute.first.first, first_attribute.second);
  bool is_conversion_needed_second = Utils::IsConversionNeeded(second_attribute.first.first, second_attribute.second);

  if (is_conversion_needed_first && is_conversion_needed_second) {
    single_constraints_first = pkb->GetName(GetStmtType(first_attribute.first.first));
    single_constraints_second = pkb->GetName(GetStmtType(second_attribute.first.first));
    std::unordered_set<std::string> single_column_intersection =
        HandleSetIntersectionSingleColumn(single_constraints_first, single_constraints_second);
    std::unordered_set<std::string> intersection_first;
    std::unordered_set<std::string> intersection_second;
    for (const auto &s : single_column_intersection) {
      std::unordered_set<std::string> all_stmts_first = pkb->GetStmtByName(GetStmtType(first_attribute.first.first), s);
      std::unordered_set<std::string>
          all_stmts_second = pkb->GetStmtByName(GetStmtType(second_attribute.first.first), s);
      intersection_first.insert(all_stmts_first.begin(), all_stmts_first.end());
      intersection_second.insert(all_stmts_second.begin(), all_stmts_second.end());
    }

    auto it1 = intersection_first.begin();
    auto it2 = intersection_second.begin();
    for (; it1!=intersection_first.end() && it2!=intersection_second.end(); ++it1, ++it2) {
      intersection_pair.insert(std::make_pair(*it1, *it2));
    }
  } else if (is_conversion_needed_first) {
    single_constraints_first = pkb->GetName(GetStmtType(first_attribute.first.first));
    single_constraints_second = pkb->GetStmt(GetStmtType(second_attribute.first.first));
    std::unordered_set<std::string> single_column_intersection =
        HandleSetIntersectionSingleColumn(single_constraints_first, single_constraints_second);
    std::unordered_set<std::string> intersection_temp;
    for (const auto &s : single_column_intersection) {
      std::unordered_set<std::string> all_stmts = pkb->GetStmtByName(GetStmtType(first_attribute.first.first), s);
      for (const auto &stmt_no : all_stmts) {
        intersection_pair.insert(std::make_pair(stmt_no, s));
      }
    }

  } else if (is_conversion_needed_second) {
    single_constraints_second = pkb->GetName(GetStmtType(second_attribute.first.first));
    single_constraints_first = pkb->GetStmt(GetStmtType(first_attribute.first.first));
    std::unordered_set<std::string> single_column_intersection =
        HandleSetIntersectionSingleColumn(single_constraints_first, single_constraints_second);
    std::unordered_set<std::string> intersection_temp;
    for (const auto &s : single_column_intersection) {
      std::unordered_set<std::string> all_stmts = pkb->GetStmtByName(GetStmtType(second_attribute.first.first), s);
      for (const auto &stmt_no : all_stmts) {
        intersection_pair.insert(std::make_pair(s, stmt_no));
      }
    }
  } else {
    single_constraints_first = pkb->GetStmt(GetStmtType(first_attribute.first.first));
    single_constraints_second = pkb->GetStmt(GetStmtType(second_attribute.first.first));
    intersection_pair = HandleSetIntersection(single_constraints_first, single_constraints_second);
  }

  return intersection_pair.empty();
}

bool WithClause::HandleAttributeIdentBool() {
  std::pair<std::pair<DesignEntityType, std::string>, AttriName>
      first_attribute = Utils::ParseAttributeRef(first_arg, declarations);
  std::unordered_set<std::string> single_constraints;
  Table table;
  bool is_conversion_needed = Utils::IsConversionNeeded(first_attribute.first.first, first_attribute.second);
  if (is_conversion_needed) {
    single_constraints = pkb->GetStmtByName(GetStmtType(first_attribute.first.first), second_arg.value);
    if (single_constraints.empty()) {
      return true;
    }
  } else {
    single_constraints = pkb->GetStmt(GetStmtType(first_attribute.first.first));
    if (single_constraints.count(second_arg.value)==0) {
      return true;
    }
  }

  return false;
}

bool WithClause::HandleAttributeIntegerBool() {
  std::pair<std::pair<DesignEntityType, std::string>, AttriName>
      first_attribute = Utils::ParseAttributeRef(first_arg, declarations);
  auto single_constraints = pkb->GetStmt(GetStmtType(first_attribute.first.first));
  if (single_constraints.count(second_arg.value)==0) {
    return true;
  }
  return false;
}

bool WithClause::HandleIntegerAttributeBool() {
  std::pair<std::pair<DesignEntityType, std::string>, AttriName>
      second_attribute = Utils::ParseAttributeRef(second_arg, declarations);
  auto single_constraints = pkb->GetStmt(GetStmtType(second_attribute.first.first));
  if (single_constraints.count(first_arg.value)==0) {
    return true;
  }
  return false;
}

bool WithClause::HandleIntegerIntegerBool() {
  bool is_false_clause = first_arg.value!=second_arg.value;
  return is_false_clause;
}

bool WithClause::HandleIdentAttributeBool() {
  std::pair<std::pair<DesignEntityType, std::string>, AttriName>
      second_attribute = Utils::ParseAttributeRef(second_arg, declarations);
  std::unordered_set<std::string> single_constraints;
  bool is_conversion_needed = Utils::IsConversionNeeded(second_attribute.first.first, second_attribute.second);
  if (is_conversion_needed) {
    single_constraints = pkb->GetStmtByName(GetStmtType(second_attribute.first.first), first_arg.value);
    return single_constraints.empty();
  } else {
    single_constraints = pkb->GetStmt(GetStmtType(second_attribute.first.first));
    if (single_constraints.count(first_arg.value)==0) {
      return true;
    }
  }
  return false;
}

bool WithClause::HandleIdentIdentBool() {
  // same logic as HandleIntegerInteger
  return HandleIntegerIntegerBool();
}

std::unordered_set<std::string> WithClause::HandleSetIntersectionSingleColumn(std::unordered_set<std::string> set_one,
                                                                              std::unordered_set<std::string> set_two) {
  std::unordered_set<std::string> intersection_pair;
  for (const auto &s : set_one) {
    if (set_two.count(s)!=0) {
      intersection_pair.insert(s);
    }
  }
  return intersection_pair;
}

std::unordered_set<std::pair<std::string, std::string>,
                   pair_hash> WithClause::HandleSetIntersection(const std::unordered_set<
    std::string> &set_one, const std::unordered_set<std::string> &set_two) {
  std::unordered_set<std::pair<std::string, std::string>, pair_hash> intersection_pair;
  for (const auto &s : set_one) {
    if (set_two.count(s)!=0) {
      intersection_pair.insert(std::make_pair(s, s));
    }
  }

  return intersection_pair;
}

std::set<std::string> WithClause::GetSynonyms() {
  std::set<std::string> synonyms;
  if (IsArgAttribute(first_arg)) {
    auto attribute = Utils::ParseAttributeRef(first_arg, declarations);
    synonyms.emplace(attribute.first.second);
  }
  if (IsArgAttribute(second_arg)) {
    auto attribute = Utils::ParseAttributeRef(second_arg, declarations);
    synonyms.emplace(attribute.first.second);
  }
  return synonyms;
}

size_t WithClause::GetSynonymsSize() {
  size_t size = 0;
  if (IsArgAttribute(first_arg)) {
    size++;
  }
  if (IsArgAttribute(second_arg)) {
    size++;
  }
  return size;
}

size_t WithClause::GetWeight() {
  return weight;
}

}