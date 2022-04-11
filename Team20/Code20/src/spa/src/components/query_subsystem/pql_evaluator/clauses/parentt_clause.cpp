#include "parentt_clause.h"
#include "components/query_subsystem/pql_evaluator/clause_util.h"
#include "components/pkb/stores/parent_store/parent_store.h"

namespace pql {

using namespace clause_util;

ParentTClause::ParentTClause(const std::unordered_map<std::string, DesignEntityType> &declarations,
                             const PqlToken &first_arg,
                             const PqlToken &second_arg,
                             const pkb::PkbPtr &pkb)
    : declarations(declarations), first_arg(first_arg), second_arg(second_arg), pkb(pkb) {}

Table ParentTClause::Execute() {
  return (this->*execute_handler.at({first_arg.type, second_arg.type}))();
}

Table ParentTClause::HandleSynonymSynonym() {
  if (first_arg.value==second_arg.value) {
    return ConstructEmptyTable();
  }

  auto pair_constraints = pkb->GetParentStore()->GetAllParentStarStmt(
      GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
      GetStmtType(GetSynonymDesignEntity(second_arg, declarations))
  );
  return {first_arg.value, second_arg.value, pair_constraints};
}

Table ParentTClause::HandleSynonymWildcard() {
  auto pair_constraints = pkb->GetParentStore()->GetAllParentStarStmt(
      GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
      StmtType::STMT
  );
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }
  return {first_arg.value, single_constraints};
}

Table ParentTClause::HandleSynonymInteger() {
  auto single_constraints =
      pkb->GetParentStore()->GetAllAnceOf(GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
                                          second_arg.value);
  return {first_arg.value, single_constraints};
}

Table ParentTClause::HandleWildcardSynonym() {
  auto pair_constraints = pkb->GetParentStore()->GetAllParentStarStmt(
      StmtType::STMT,
      GetStmtType(GetSynonymDesignEntity(second_arg, declarations))
  );
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.second);
  }
  return {second_arg.value, single_constraints};
}

Table ParentTClause::HandleWildcardWildcard() {
  bool is_false_clause = pkb->GetParentStore()->GetAllParentStarStmt(StmtType::STMT, StmtType::STMT).empty();
  return ConstructEmptyTable(is_false_clause);
}

Table ParentTClause::HandleWildcardInteger() {
  bool is_false_clause = pkb->GetParentStore()->GetAllAnceOf(STMT, second_arg.value).empty();
  return ConstructEmptyTable(is_false_clause);
}

Table ParentTClause::HandleIntegerSynonym() {
  auto single_constraints =
      pkb->GetParentStore()->GetAllDescOf(GetStmtType(GetSynonymDesignEntity(second_arg, declarations)),
                                          first_arg.value);
  return {second_arg.value, single_constraints};
}

Table ParentTClause::HandleIntegerWildcard() {
  bool is_false_clause = pkb->GetParentStore()->GetAllDescOf(STMT, first_arg.value).empty();
  return ConstructEmptyTable(is_false_clause);
}

Table ParentTClause::HandleIntegerInteger() {
  bool is_false_clause = !pkb->GetParentStore()->IsAnceDescValid({first_arg.value, second_arg.value});
  return ConstructEmptyTable(is_false_clause);
}

bool ParentTClause::ExecuteBool() {
  return (this->*execute_bool_handler.at({first_arg.type, second_arg.type}))();
}

bool ParentTClause::HandleSynonymSynonymBool() {
  if (first_arg.value==second_arg.value) {
    return true;
  }

  auto pair_constraints = pkb->GetParentStore()->GetAllParentStarStmt(
      GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
      GetStmtType(GetSynonymDesignEntity(second_arg, declarations))
  );
  return pair_constraints.empty();
}

bool ParentTClause::HandleSynonymWildcardBool() {
  auto pair_constraints = pkb->GetParentStore()->GetAllParentStarStmt(
      GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
      StmtType::STMT
  );
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }
  return single_constraints.empty();
}

bool ParentTClause::HandleSynonymIntegerBool() {
  auto single_constraints =
      pkb->GetParentStore()->GetAllAnceOf(GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
                                          second_arg.value);
  return single_constraints.empty();
}

bool ParentTClause::HandleWildcardSynonymBool() {
  auto pair_constraints = pkb->GetParentStore()->GetAllParentStarStmt(
      StmtType::STMT,
      GetStmtType(GetSynonymDesignEntity(second_arg, declarations))
  );
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.second);
  }
  return single_constraints.empty();
}

bool ParentTClause::HandleWildcardWildcardBool() {
  bool is_false_clause = pkb->GetParentStore()->GetAllParentStarStmt(StmtType::STMT, StmtType::STMT).empty();
  return is_false_clause;
}

bool ParentTClause::HandleWildcardIntegerBool() {
  bool is_false_clause = pkb->GetParentStore()->GetAllAnceOf(STMT, second_arg.value).empty();
  return is_false_clause;
}

bool ParentTClause::HandleIntegerSynonymBool() {
  auto single_constraints =
      pkb->GetParentStore()->GetAllDescOf(GetStmtType(GetSynonymDesignEntity(second_arg, declarations)),
                                          first_arg.value);
  return single_constraints.empty();
}

bool ParentTClause::HandleIntegerWildcardBool() {
  bool is_false_clause = pkb->GetParentStore()->GetAllDescOf(STMT, first_arg.value).empty();
  return is_false_clause;
}

bool ParentTClause::HandleIntegerIntegerBool() {
  bool is_false_clause = !pkb->GetParentStore()->IsAnceDescValid({first_arg.value, second_arg.value});
  return is_false_clause;
}

std::set<std::string> ParentTClause::GetSynonyms() {
  std::set<std::string> synonyms;
  if (IsArgSynonym(first_arg)) {
    synonyms.emplace(first_arg.value);
  }
  if (IsArgSynonym(second_arg)) {
    synonyms.emplace(second_arg.value);
  }

  return synonyms;
}

size_t ParentTClause::GetSynonymsSize() {
  size_t size = 0;
  if (IsArgSynonym(first_arg)) {
    size++;
  }
  if (IsArgSynonym(second_arg)) {
    size++;
  }
  return size;
}

size_t ParentTClause::GetWeight() {
  return weight;
}

}
