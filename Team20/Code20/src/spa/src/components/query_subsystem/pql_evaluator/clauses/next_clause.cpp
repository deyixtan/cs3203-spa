#include "next_clause.h"
#include "components/query_subsystem/pql_evaluator/clause_util.h"
#include "components/pkb/stores/next_store/next_store.h"

namespace pql {

using namespace clause_util;

NextClause::NextClause(const std::unordered_map<std::string, DesignEntityType> &declarations,
                       const PqlToken &first_arg,
                       const PqlToken &second_arg,
                       const pkb::PkbPtr &pkb)
    : declarations(declarations), first_arg(first_arg), second_arg(second_arg), pkb(pkb) {}

Table NextClause::Execute() {
  return (this->*execute_handler.at({first_arg.type, second_arg.type}))();
}

Table NextClause::HandleSynonymSynonym() {
  if (first_arg.value==second_arg.value) {
    return ConstructEmptyTable();
  }

  auto pair_constraints = pkb->GetNextStore()->GetAllNextStmt(
      GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
      GetStmtType(GetSynonymDesignEntity(second_arg, declarations))
  );
  return {first_arg.value, second_arg.value, pair_constraints};
}

Table NextClause::HandleSynonymWildcard() {
  auto pair_constraints = pkb->GetNextStore()->GetAllNextStmt(
      GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
      StmtType::STMT
  );
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }
  return {first_arg.value, single_constraints};
}

Table NextClause::HandleSynonymInteger() {
  std::unordered_set<std::string> single_constraints =
      pkb->GetNextStore()->GetBeforeOf(GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
                                       second_arg.value);
  return {first_arg.value, single_constraints};
}

Table NextClause::HandleWildcardSynonym() {
  auto pair_constraints = pkb->GetNextStore()->GetAllNextStmt(
      StmtType::STMT,
      GetStmtType(GetSynonymDesignEntity(second_arg, declarations))
  );
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.second);
  }
  return {second_arg.value, single_constraints};
}

Table NextClause::HandleWildcardWildcard() {
  bool is_false_clause = pkb->GetNextStore()->GetAllNextStmt(StmtType::STMT, StmtType::STMT).empty();
  return ConstructEmptyTable(is_false_clause);
}

Table NextClause::HandleWildcardInteger() {
  bool is_false_clause = pkb->GetNextStore()->GetBeforeOf(STMT, second_arg.value).empty();
  return ConstructEmptyTable(is_false_clause);
}

Table NextClause::HandleIntegerSynonym() {
  std::unordered_set<std::string> single_constraints =
      pkb->GetNextStore()->GetNextOf(GetStmtType(GetSynonymDesignEntity(second_arg, declarations)),
                                     first_arg.value);
  return {second_arg.value, single_constraints};
}

Table NextClause::HandleIntegerWildcard() {
  bool is_false_clause = pkb->GetNextStore()->GetNextOf(STMT, first_arg.value).empty();
  return ConstructEmptyTable(is_false_clause);
}

Table NextClause::HandleIntegerInteger() {
  bool is_false_clause = !pkb->GetNextStore()->IsNextPairValid({first_arg.value, second_arg.value});
  Table table;
  if (is_false_clause) {
    table.ToggleFalseClause();
  }
  return table;
}

bool NextClause::ExecuteBool() {
  return (this->*execute_bool_handler.at({first_arg.type, second_arg.type}))();
}

bool NextClause::HandleSynonymSynonymBool() {
  if (first_arg.value==second_arg.value) {
    return true;
  }

  auto pair_constraints = pkb->GetNextStore()->GetAllNextStmt(
      GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
      GetStmtType(GetSynonymDesignEntity(second_arg, declarations))
  );
  return pair_constraints.empty();
}

bool NextClause::HandleSynonymWildcardBool() {
  auto pair_constraints = pkb->GetNextStore()->GetAllNextStmt(
      GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
      StmtType::STMT
  );
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }
  return single_constraints.empty();
}

bool NextClause::HandleSynonymIntegerBool() {
  std::unordered_set<std::string> single_constraints =
      pkb->GetNextStore()->GetBeforeOf(GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
                                       second_arg.value);
  return single_constraints.empty();
}

bool NextClause::HandleWildcardSynonymBool() {
  auto pair_constraints = pkb->GetNextStore()->GetAllNextStmt(
      StmtType::STMT,
      GetStmtType(GetSynonymDesignEntity(second_arg, declarations))
  );
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.second);
  }
  return single_constraints.empty();
}

bool NextClause::HandleWildcardWildcardBool() {
  bool is_false_clause = pkb->GetNextStore()->GetAllNextStmt(StmtType::STMT, StmtType::STMT).empty();
  return is_false_clause;
}

bool NextClause::HandleWildcardIntegerBool() {
  bool is_false_clause = pkb->GetNextStore()->GetBeforeOf(STMT, second_arg.value).empty();
  return is_false_clause;
}

bool NextClause::HandleIntegerSynonymBool() {
  std::unordered_set<std::string> single_constraints =
      pkb->GetNextStore()->GetNextOf(GetStmtType(GetSynonymDesignEntity(second_arg, declarations)),
                                     first_arg.value);
  return single_constraints.empty();
}

bool NextClause::HandleIntegerWildcardBool() {
  bool is_false_clause = pkb->GetNextStore()->GetNextOf(STMT, first_arg.value).empty();
  return is_false_clause;
}

bool NextClause::HandleIntegerIntegerBool() {
  bool is_false_clause = !pkb->GetNextStore()->IsNextPairValid({first_arg.value, second_arg.value});
  return is_false_clause;
}

std::set<std::string> NextClause::GetSynonyms() {
  std::set<std::string> synonyms;
  if (IsArgSynonym(first_arg)) {
    synonyms.emplace(first_arg.value);
  }
  if (IsArgSynonym(second_arg)) {
    synonyms.emplace(second_arg.value);
  }

  return synonyms;
}

size_t NextClause::GetSynonymsSize() {
  size_t size = 0;
  if (IsArgSynonym(first_arg)) {
    size++;
  }
  if (IsArgSynonym(second_arg)) {
    size++;
  }
  return size;
}

size_t NextClause::GetWeight() {
  return weight;
}

}