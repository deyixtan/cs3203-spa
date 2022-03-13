#include "parentt_clause.h"
#include "clause_util.h"

namespace pql {

using namespace clause_util;

ParentTClause::ParentTClause(const std::unordered_map<std::string, DesignEntityType> &declarations,
                             const PqlToken &first_arg,
                             const PqlToken &second_arg,
                             PKB *pkb)
    : declarations(declarations), first_arg(first_arg), second_arg(second_arg), pkb(pkb) {}

Table ParentTClause::Execute() {
  if (IsArgSynonym(first_arg) && IsArgSynonym(second_arg)) {
    // Parent(s1, s2)
    return HandleSynonymSynonym();
  } else if (IsArgSynonym(first_arg) && IsArgWildcard(second_arg)) {
    // Parent(s, _)
    return HandleSynonymWildcard();
  } else if (IsArgSynonym(first_arg) && IsArgInteger(second_arg)) {
    // Parent(s, 2)
    return HandleSynonymInteger();
  } else if (IsArgWildcard(first_arg) && IsArgSynonym(second_arg)) {
    // Parent(_, s)
    return HandleWildcardSynonym();
  } else if (IsArgWildcard(first_arg) && IsArgWildcard(second_arg)) {
    // Parent(_, _)
    return HandleWildcardWildcard();
  } else if (IsArgWildcard(first_arg) && IsArgInteger(second_arg)) {
    // Parent(_, 2)
    return HandleWildcardInteger();
  } else if (IsArgInteger(first_arg) && IsArgSynonym(second_arg)) {
    // Parent(1, s)
    return HandleIntegerSynonym();
  } else if (IsArgInteger(first_arg) && IsArgWildcard(second_arg)) {
    // Parent(1, _)
    return HandleIntegerWildcard();
  } else if (IsArgInteger(first_arg) && IsArgInteger(second_arg)) {
    // Parent(1, 2)
    return HandleIntegerInteger();
  } else {
    // throw exception???
    // return empty table???
    return {};
  }
}

Table ParentTClause::HandleSynonymSynonym() {
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
  auto pair_constraints = pkb->GetParentStore()->GetAllParentStarStmt(
      GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
      StmtType::STMT
  );
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    if (pair_constraint.second==second_arg.value) {
      single_constraints.insert(pair_constraint.first);
    }
  }
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
  Table table;
  if (is_false_clause) {
    table.EncounteredFalseClause();
  }
  return table;
}

Table ParentTClause::HandleWildcardInteger() {
  auto pair_constraints = pkb->GetParentStore()->GetAllParentStarStmt(StmtType::STMT, StmtType::STMT);
  bool is_false_clause = true;
  for (const auto &pair_constraint : pair_constraints) {
    if (pair_constraint.second==second_arg.value) {
      is_false_clause = false;
    }
  }
  Table table;
  if (is_false_clause) {
    table.EncounteredFalseClause();
  }
  return table;
}

Table ParentTClause::HandleIntegerSynonym() {
  auto pair_constraints = pkb->GetParentStore()->GetAllParentStarStmt(
      StmtType::STMT,
      GetStmtType(GetSynonymDesignEntity(second_arg, declarations))
  );
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    if (pair_constraint.first==first_arg.value) {
      single_constraints.insert(pair_constraint.second);
    }
  }
  return {second_arg.value, single_constraints};
}

Table ParentTClause::HandleIntegerWildcard() {
  bool is_false_clause = pkb->GetParentStore()->GetAllDescOf(first_arg.value).empty();
  Table table;
  if (is_false_clause) {
    table.EncounteredFalseClause();
  }
  return table;
}

Table ParentTClause::HandleIntegerInteger() {
  auto descendants = pkb->GetParentStore()->GetAllDescOf(first_arg.value);
  bool is_false_clause = descendants.find(second_arg.value)==descendants.end();
  Table table;
  if (is_false_clause) {
    table.EncounteredFalseClause();
  }
  return table;
}

}