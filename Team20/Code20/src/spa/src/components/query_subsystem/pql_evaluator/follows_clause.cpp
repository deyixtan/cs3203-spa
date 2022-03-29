#include "follows_clause.h"
#include "clause_util.h"

namespace pql {

using namespace clause_util;

FollowsClause::FollowsClause(const std::unordered_map<std::string, DesignEntityType> &declarations,
                             const PqlToken &first_arg,
                             const PqlToken &second_arg,
                             PKB *pkb)
    : declarations(declarations), first_arg(first_arg), second_arg(second_arg), pkb(pkb) {}

Table FollowsClause::Execute() {
  if (IsArgSynonym(first_arg) && IsArgSynonym(second_arg)) {
    // Follows(s1, s2)
    return HandleSynonymSynonym();
  } else if (IsArgSynonym(first_arg) && IsArgWildcard(second_arg)) {
    // Follows(s, _)
    return HandleSynonymWildcard();
  } else if (IsArgSynonym(first_arg) && IsArgInteger(second_arg)) {
    // Follows(s, 2)
    return HandleSynonymInteger();
  } else if (IsArgWildcard(first_arg) && IsArgSynonym(second_arg)) {
    // Follows(_, s)
    return HandleWildcardSynonym();
  } else if (IsArgWildcard(first_arg) && IsArgWildcard(second_arg)) {
    // Follows(_, _)
    return HandleWildcardWildcard();
  } else if (IsArgWildcard(first_arg) && IsArgInteger(second_arg)) {
    // Follows(_, 2)
    return HandleWildcardInteger();
  } else if (IsArgInteger(first_arg) && IsArgSynonym(second_arg)) {
    // Follows(1, s)
    return HandleIntegerSynonym();
  } else if (IsArgInteger(first_arg) && IsArgWildcard(second_arg)) {
    // Follows(1, _)
    return HandleIntegerWildcard();
  } else if (IsArgInteger(first_arg) && IsArgInteger(second_arg)) {
    // Follows(1, 2)
    return HandleIntegerInteger();
  } else {
    // throw exception???
    // return empty table???
    return {};
  }
}

Table FollowsClause::HandleSynonymSynonym() {
  auto pair_constraints = pkb->GetFollowsStore()->GetAllFollowStmt(
      GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
      GetStmtType(GetSynonymDesignEntity(second_arg, declarations))
  );
  return {first_arg.value, second_arg.value, pair_constraints};
}

Table FollowsClause::HandleSynonymWildcard() {
  auto pair_constraints = pkb->GetFollowsStore()->GetAllFollowStmt(
      GetStmtType(GetSynonymDesignEntity(first_arg, declarations)),
      StmtType::STMT
  );
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.first);
  }
  return {first_arg.value, single_constraints};
}

Table FollowsClause::HandleSynonymInteger() {
  auto pair_constraints = pkb->GetFollowsStore()->GetAllFollowStmt(
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

Table FollowsClause::HandleWildcardSynonym() {
  auto pair_constraints = pkb->GetFollowsStore()->GetAllFollowStmt(
      StmtType::STMT,
      GetStmtType(GetSynonymDesignEntity(second_arg, declarations))
  );
  std::unordered_set<std::string> single_constraints;
  for (const auto &pair_constraint : pair_constraints) {
    single_constraints.insert(pair_constraint.second);
  }
  return {second_arg.value, single_constraints};
}

Table FollowsClause::HandleWildcardWildcard() {
  bool is_false_clause = pkb->GetFollowsStore()->GetAllFollowStmt(StmtType::STMT, StmtType::STMT).empty();
  Table table;
  if (is_false_clause) {
    table.ToggleFalseClause();
  }
  return table;
}

Table FollowsClause::HandleWildcardInteger() {
  auto pair_constraints = pkb->GetFollowsStore()->GetAllFollowStmt(StmtType::STMT, StmtType::STMT);
  bool is_false_clause = true;
  for (const auto& pair_constraint : pair_constraints) {
    if (pair_constraint.second==second_arg.value) {
      is_false_clause = false;
    }
  }
  Table table;
  if (is_false_clause) {
    table.ToggleFalseClause();
  }
  return table;
}

Table FollowsClause::HandleIntegerSynonym() {
  auto pair_constraints = pkb->GetFollowsStore()->GetAllFollowStmt(
      StmtType::STMT,
      GetStmtType(GetSynonymDesignEntity(second_arg, declarations))
  );
  std::unordered_set<std::string> single_constraints;
  for (const auto& pair_constraint : pair_constraints) {
    if (pair_constraint.first==first_arg.value) {
      single_constraints.insert(pair_constraint.second);
    }
  }
  return {second_arg.value, single_constraints};
}

Table FollowsClause::HandleIntegerWildcard() {
  bool is_false_clause = pkb->GetFollowsStore()->GetFollowingOf(first_arg.value)=="0";
  Table table;
  if (is_false_clause) {
    table.ToggleFalseClause();
  }
  return table;
}

Table FollowsClause::HandleIntegerInteger() {
  bool is_false_clause = pkb->GetFollowsStore()->GetFollowingOf(first_arg.value)!=second_arg.value;
  Table table;
  if (is_false_clause) {
    table.ToggleFalseClause();
  }
  return table;
}

}

