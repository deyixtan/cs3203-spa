#include "parent_clause.h"
#include "clause_util.h"

namespace pql {

using namespace clause_util;

ParentClause::ParentClause(const std::unordered_map<std::string, DesignEntityType> &declarations,
                           const PqlToken &first_arg,
                           const PqlToken &second_arg,
                           const pkb::PkbPtr &pkb)
    : declarations(declarations), first_arg(first_arg), second_arg(second_arg), pkb(pkb) {}

Table ParentClause::Execute() {
  return (this->*execute_handler.at({first_arg.type, second_arg.type}))();
}

Table ParentClause::HandleSynonymSynonym() {
  if (first_arg.value==second_arg.value) {
    return ConstructEmptyTable();
  }

  auto pair_constraints = pkb->GetParentStore()->GetAllParentStmt(
      GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
      GetStmtType(GetSynonymDesignEntity(second_arg, declarations))
  );
  return {first_arg.value, second_arg.value, pair_constraints};
}

Table ParentClause::HandleSynonymWildcard() {
  auto pair_constraints = pkb->GetParentStore()->GetAllParentStmt(
      GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
      StmtType::STMT
  );
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }
  return {first_arg.value, single_constraints};
}

Table ParentClause::HandleSynonymInteger() {
  std::unordered_set<std::string> single_constraints;
  std::string parent =
      pkb->GetParentStore()->GetParentOf(GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
                                         second_arg.value);
  if (parent!="0") {
    single_constraints.emplace(parent);
  }
  return {first_arg.value, single_constraints};
}

Table ParentClause::HandleWildcardSynonym() {
  auto pair_constraints = pkb->GetParentStore()->GetAllParentStmt(
      StmtType::STMT,
      GetStmtType(GetSynonymDesignEntity(second_arg, declarations))
  );
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.second);
  }
  return {second_arg.value, single_constraints};
}

Table ParentClause::HandleWildcardWildcard() {
  bool is_false_clause = pkb->GetParentStore()->GetAllParentStmt(StmtType::STMT, StmtType::STMT).empty();
  Table table;
  if (is_false_clause) {
    table.ToggleFalseClause();
  }
  return table;
}

Table ParentClause::HandleWildcardInteger() {
  bool is_false_clause = pkb->GetParentStore()->GetParentOf(STMT, second_arg.value)=="0";
  Table table;
  if (is_false_clause) {
    table.ToggleFalseClause();
  }
  return table;
}

Table ParentClause::HandleIntegerSynonym() {
  auto single_constraints =
      pkb->GetParentStore()->GetChildOf(GetStmtType(GetSynonymDesignEntity(second_arg, declarations)),
                                        first_arg.value);
  return {second_arg.value, single_constraints};
}

Table ParentClause::HandleIntegerWildcard() {
  bool is_false_clause = pkb->GetParentStore()->GetChildOf(STMT, first_arg.value).empty();
  Table table;
  if (is_false_clause) {
    table.ToggleFalseClause();
  }
  return table;
}

Table ParentClause::HandleIntegerInteger() {
  bool is_false_clause = !pkb->GetParentStore()->IsParentPairValid({first_arg.value, second_arg.value});
  Table table;
  if (is_false_clause) {
    table.ToggleFalseClause();
  }
  return table;
}

bool ParentClause::ExecuteBool() {
  return (this->*execute_bool_handler.at({first_arg.type, second_arg.type}))();
}

bool ParentClause::HandleSynonymSynonymBool() {
  if (first_arg.value==second_arg.value) {
    return true;
  }

  auto pair_constraints = pkb->GetParentStore()->GetAllParentStmt(
      GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
      GetStmtType(GetSynonymDesignEntity(second_arg, declarations))
  );
  return pair_constraints.empty();
}

bool ParentClause::HandleSynonymWildcardBool() {
  auto pair_constraints = pkb->GetParentStore()->GetAllParentStmt(
      GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
      StmtType::STMT
  );
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }
  return single_constraints.empty();
}

bool ParentClause::HandleSynonymIntegerBool() {
  std::unordered_set<std::string> single_constraints;
  std::string parent =
      pkb->GetParentStore()->GetParentOf(GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
                                         second_arg.value);
  if (parent!="0") {
    single_constraints.emplace(parent);
  }
  return single_constraints.empty();
}

bool ParentClause::HandleWildcardSynonymBool() {
  auto pair_constraints = pkb->GetParentStore()->GetAllParentStmt(
      StmtType::STMT,
      GetStmtType(GetSynonymDesignEntity(second_arg, declarations))
  );
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.second);
  }
  return single_constraints.empty();
}

bool ParentClause::HandleWildcardWildcardBool() {
  bool is_false_clause = pkb->GetParentStore()->GetAllParentStmt(StmtType::STMT, StmtType::STMT).empty();
  return is_false_clause;
}

bool ParentClause::HandleWildcardIntegerBool() {
  bool is_false_clause = pkb->GetParentStore()->GetParentOf(STMT, second_arg.value)=="0";
  return is_false_clause;
}

bool ParentClause::HandleIntegerSynonymBool() {
  auto single_constraints =
      pkb->GetParentStore()->GetChildOf(GetStmtType(GetSynonymDesignEntity(second_arg, declarations)),
                                        first_arg.value);
  return single_constraints.empty();
}

bool ParentClause::HandleIntegerWildcardBool() {
  bool is_false_clause = pkb->GetParentStore()->GetChildOf(STMT, first_arg.value).empty();
  return is_false_clause;
}

bool ParentClause::HandleIntegerIntegerBool() {
  bool is_false_clause = !pkb->GetParentStore()->IsParentPairValid({first_arg.value, second_arg.value});
  return is_false_clause;
}

std::set<std::string> ParentClause::GetSynonyms() {
  std::set<std::string> synonyms;
  if (IsArgSynonym(first_arg)) {
    synonyms.emplace(first_arg.value);
  }
  if (IsArgSynonym(second_arg)) {
    synonyms.emplace(second_arg.value);
  }

  return synonyms;
}

size_t ParentClause::GetSynonymsSize() {
  size_t size = 0;
  if (IsArgSynonym(first_arg)) {
    size++;
  }
  if (IsArgSynonym(second_arg)) {
    size++;
  }
  return size;
}

size_t ParentClause::GetWeight() {
  return weight;
}
}